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

namespace Envoy {
namespace Http {

PolicyEnforcementFilterConfig::PolicyEnforcementFilterConfig(const Json::Object& json_config, Runtime::Loader& runtime,
                                     const std::string& stat_prefix, Stats::Scope& stats)
    : runtime_(runtime), stats_(generateStats(stat_prefix, stats)) {


  const Json::ObjectSharedPtr config_abort = json_config.getObject("abort", true);


  if (!config_abort->empty()) {

    // TODO(mattklein123): Throw error if invalid return code is provided
    http_status_ = static_cast<uint64_t>(config_abort->getInteger("http_status"));
  }

  if (json_config.hasObject("headers")) {
    std::vector<Json::ObjectSharedPtr> config_headers = json_config.getObjectArray("headers");
    for (const Json::ObjectSharedPtr& header_map : config_headers) {
      pe_filter_headers_.push_back(*header_map);
    }
  }

}

PolicyEnforcementFilter::PolicyEnforcementFilter(PolicyEncorcementFilterConfigSharedPtr config) : config_(config) {}

PolicyEnforcementFilter::~PolicyEnforcementFilter() {}

FilterHeadersStatus PolicyEnforcementFilter::decodeHeaders(HeaderMap&, bool) {



//  // Check for header matches
//  if (!Router::ConfigUtility::matchHeaders(headers, config_->filterHeaders())) {
//    return FilterHeadersStatus::Continue;
//  }
//
//
//  if (config_->runtime().snapshot().featureEnabled("pe.http.abort.abort_percent",
//                                                   config_->abortPercent())) {
//    abortWithHTTPStatus();
//    return FilterHeadersStatus::StopIteration;
//  }

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
  // TODO(mattklein123): check http status codes obtained from runtime
  Http::HeaderMapPtr response_headers{new HeaderMapImpl{
      {Headers::get().Status, std::to_string(config_->runtime().snapshot().getInteger(
                                  "pe.http.abort.http_status", config_->abortCode()))}}};
  callbacks_->encodeHeaders(std::move(response_headers), true);
  config_->stats().aborts_injected_.inc();
  callbacks_->requestInfo().setResponseFlag(Http::AccessLog::ResponseFlag::FaultInjected);
}



void PolicyEnforcementFilter::setDecoderFilterCallbacks(StreamDecoderFilterCallbacks& callbacks) {
  callbacks_ = &callbacks;
}

} // Http
} // Envoy
