#include "common/http/filter/policy_enforcement_filter.h"

#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

#include "envoy/event/timer.h"
#include "envoy/http/codes.h"
#include "envoy/http/header_map.h"
#include "envoy/stats/stats.h"

#include "common/common/assert.h"
#include "common/common/empty_string.h"
#include "common/http/codes.h"
#include "common/http/header_map_impl.h"
#include "common/http/headers.h"
#include "common/json/config_schemas.h"
#include "common/router/config_impl.h"
#include "common/common/enum_to_int.h"

#include "spdlog/spdlog.h"

namespace Envoy {
namespace Http {

PolicyEnforcementFilterConfig::PolicyEnforcementFilterConfig(const Json::Object&, Runtime::Loader& runtime,
                                     const std::string& stat_prefix, Stats::Scope& stats)
    : runtime_(runtime), stats_(generateStats(stat_prefix, stats)) {


}

PolicyEnforcementFilter::PolicyEnforcementFilter(PolicyEncorcementFilterConfigSharedPtr config) : config_(config) {}

PolicyEnforcementFilter::~PolicyEnforcementFilter() {}

FilterHeadersStatus PolicyEnforcementFilter::decodeHeaders(HeaderMap& headers, bool) {

  // check for Authorization header
  const Http::HeaderEntry* auth_header = headers.get(Http::LowerCaseString("Authorization"));
  if (auth_header == nullptr) {
//    ENVOY_LOG(warn, "Aborting request");
    abortWithHTTPStatus();
    return FilterHeadersStatus::StopIteration;
  }


  return FilterHeadersStatus::Continue;
}



FilterDataStatus PolicyEnforcementFilter::decodeData(Buffer::Instance&, bool) {
  return FilterDataStatus::Continue;
}

FilterTrailersStatus PolicyEnforcementFilter::decodeTrailers(HeaderMap&) {
  return FilterTrailersStatus::Continue;
}

PolicyEnforcementFilterStats PolicyEnforcementFilterConfig::generateStats(const std::string& prefix, Stats::Scope& scope) {
  std::string final_prefix = prefix + "pe.";
  return {ALL_PE_FILTER_STATS(POOL_COUNTER_PREFIX(scope, final_prefix))};
}

void PolicyEnforcementFilter::onDestroy() { }


void PolicyEnforcementFilter::abortWithHTTPStatus() {
  Http::HeaderMapPtr response_headers{new HeaderMapImpl{
          {Headers::get().Status, std::to_string(enumToInt(Http::Code::Forbidden))}}};
  callbacks_->encodeHeaders(std::move(response_headers), true);
  config_->stats().aborts_injected_.inc();
  callbacks_->requestInfo().setResponseFlag(Http::AccessLog::ResponseFlag::FaultInjected);
}



void PolicyEnforcementFilter::setDecoderFilterCallbacks(StreamDecoderFilterCallbacks& callbacks) {
  callbacks_ = &callbacks;
}

} // Http
} // Envoy
