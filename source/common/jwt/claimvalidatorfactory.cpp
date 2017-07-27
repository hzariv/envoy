// Copyright (c) 2015 Erwin Jansen
//
// MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#include "claimvalidatorfactory.h"
#include <jansson.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "buildwrappers.h"
#include "allocators.h"
#include "listclaimvalidator.h"
#include "timevalidator.h"


ClaimValidatorFactory::~ClaimValidatorFactory() {
  for (auto it = build_.begin(); it != build_.end(); it++) {
    delete *it;
  }
}

ClaimValidator *ClaimValidatorFactory::Build(json_t *json) {
  if (json == NULL) {
    throw std::logic_error("Cannot construct from empty json!");
  }
  if (json_object_size(json) > 1) {
    char *fail = json_dumps(json, 0);
    std::ostringstream msg;
    msg << "More than one property at: " << fail;
    free(fail);
    throw std::logic_error(msg.str());
  }

  ClaimValidator *constructed = nullptr;
  if (json_object_get(json, "iss")) {
    constructed = new ListClaimValidator("iss",
                                         BuildList(json_object_get(json, "iss")));
  } else if (json_object_get(json, "sub")) {
    constructed = new ListClaimValidator("sub",
                                         BuildList(json_object_get(json, "sub")));
  } else if (json_object_get(json, "aud")) {
    constructed = new ListClaimValidator("aud",
                                         BuildList(json_object_get(json, "aud")));
  } else if (json_object_get(json, "exp")) {
    json_t *val = json_object_get(json, "exp");
    json_t *leeway = json_object_get(val, "leeway");
    constructed = new ExpValidator(json_integer_value(leeway));
  } else if (json_object_get(json, "nbf")) {
    json_t *val = json_object_get(json, "nbf");
    json_t *leeway = json_object_get(val, "leeway");
    constructed = new NbfValidator(json_integer_value(leeway));
  } else if (json_object_get(json, "iat")) {
    json_t *val = json_object_get(json, "iat");
    json_t *leeway = json_object_get(val, "leeway");
    constructed = new IatValidator(json_integer_value(leeway));
  }

  try {
    if (json_object_get(json, "all")) {
      constructed = new AllClaimValidator(BuildValidatorList(json_object_get(json, "all")));
    } else if (json_object_get(json, "any")) {
      constructed = new AnyClaimValidator(BuildValidatorList(json_object_get(json, "any")));
    } else if (json_object_get(json, "optional")) {
      json_t *val = json_object_get(json, "optional");
      ClaimValidator *inner = Build(val);
      constructed = new OptionalClaimValidator(inner);
    }
  } catch (std::exception &le) {
    std::ostringstream msg;
    msg << "Json error inside: " << le.what();
    char *fail = json_dumps(json, 0);
    if (fail) {
      msg << ", at: " << fail;
      free(fail);
    }

    throw std::logic_error(msg.str());
  }

  if (!constructed) {
    char *fail = json_dumps(json, 0);
    if (fail) {
      std::ostringstream msg;
      msg << "Missing property at: " << fail;
      free(fail);
      throw std::logic_error(msg.str());
    }
    throw std::logic_error("Missing property");
  }

  build_.push_back(constructed);
  return constructed;
}


ClaimValidator *ClaimValidatorFactory::Build(std::string fromJson) {
  json_error_t error;
  json_ptr json_str(json_loads(fromJson.c_str(), JSON_REJECT_DUPLICATES, &error));

  if (!json_str) {
    std::ostringstream msg;
    msg << "Failed to parse JSON: " << error.text
      << ", at line: " << error.line << ", col: " << error.column;
    throw std::logic_error(msg.str());
  }

  ClaimValidatorFactory factory;

  ClaimValidator *root = factory.Build(json_str.get());
  ParsedClaimvalidator *validator = new ParsedClaimvalidator(
      json_str.release(), factory.build_, root);
  factory.build_.clear();

  return validator;
}

std::vector<ClaimValidator*> ClaimValidatorFactory::BuildValidatorList(json_t *json) {
  if (!json_is_array(json)) {
    throw std::logic_error("not an array!");
  }

  size_t idx;
  json_t *value;
  std::vector<ClaimValidator*> result;

  json_array_foreach(json, idx, value) {
    result.push_back(Build(value));
  }

  return result;
}

std::vector<std::string> ClaimValidatorFactory::BuildList(json_t *object) {
  if (!json_is_array(object)) {
    throw std::logic_error("not an array!");
  }

  size_t idx;
  json_t *value;

  // Validate
  json_array_foreach(object, idx, value) {
    if (!json_is_string(value)) {
      throw std::logic_error("array can only contain strings");
    }
  }

  std::vector<std::string> result;
  json_array_foreach(object, idx, value) {
    const char *str = json_string_value(value);
    result.push_back(std::string(str));
  }

  return result;
}

ParsedClaimvalidator::ParsedClaimvalidator(json_t *json,
    const std::vector<ClaimValidator*> &children, ClaimValidator *root) :
    ClaimValidator(root->property()), json_(json), children_(children), root_(root) { }

bool ParsedClaimvalidator::IsValid(const json_t *claimset) const {
  return root_->IsValid(claimset);
}

ParsedClaimvalidator::~ParsedClaimvalidator() {
  for (auto it = children_.begin(); it != children_.end(); it++) {
    delete *it;
  }
  json_decref(json_);
}

std::string ParsedClaimvalidator::toJson() const {
  return root_->toJson();
}
