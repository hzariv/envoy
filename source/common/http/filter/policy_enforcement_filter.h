#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "envoy/http/filter.h"
#include "envoy/runtime/runtime.h"
#include "envoy/stats/stats_macros.h"

#include "common/router/config_impl.h"

namespace Envoy {
namespace Http {

/**
 * All stats for the policy enforcement filter. @see stats_macros.h
 */
// clang-format off
#define ALL_PE_FILTER_STATS(COUNTER)                                                                                                                                  \
  COUNTER(aborts_injected)
// clang-format on

/**
 * Wrapper struct for connection manager stats. @see stats_macros.h
 */
struct PolicyEnforcementFilterStats {
  ALL_PE_FILTER_STATS(GENERATE_COUNTER_STRUCT);
};

/**
 * Configuration for the policy enforcement filter.
 */
class PolicyEnforcementFilterConfig {
public:
  PolicyEnforcementFilterConfig(const Json::Object& json_config, Runtime::Loader& runtime,
                    const std::string& stat_prefix, Stats::Scope& stats);

  const std::vector<Router::ConfigUtility::HeaderData>& filterHeaders() {
    return pe_filter_headers_;
  }
  uint64_t abortCode() { return http_status_; }
  Runtime::Loader& runtime() { return runtime_; }
  PolicyEnforcementFilterStats& stats() { return stats_; }

private:
  static PolicyEnforcementFilterStats generateStats(const std::string& prefix, Stats::Scope& scope);

  uint64_t http_status_{};         // HTTP or gRPC return codes
  std::vector<Router::ConfigUtility::HeaderData> pe_filter_headers_;
  Runtime::Loader& runtime_;
  PolicyEnforcementFilterStats stats_;
};

typedef std::shared_ptr<PolicyEnforcementFilterConfig> PolicyEncorcementFilterConfigSharedPtr;

/**
 * A filter that is capable of faulting an entire request before dispatching it upstream.
 */
class PolicyEnforcementFilter : public StreamDecoderFilter {
public:
  PolicyEnforcementFilter(PolicyEncorcementFilterConfigSharedPtr config);
  ~PolicyEnforcementFilter();

  // Http::StreamFilterBase
  void onDestroy() override;

  // Http::StreamDecoderFilter
  FilterHeadersStatus decodeHeaders(HeaderMap& headers, bool end_stream) override;
  FilterDataStatus decodeData(Buffer::Instance& data, bool end_stream) override;
  FilterTrailersStatus decodeTrailers(HeaderMap& trailers) override;
  void setDecoderFilterCallbacks(StreamDecoderFilterCallbacks& callbacks) override;

private:
  void postDelayInjection();
  void abortWithHTTPStatus();

    PolicyEncorcementFilterConfigSharedPtr config_;
  StreamDecoderFilterCallbacks* callbacks_{};
};

} // Http
} // Envoy
