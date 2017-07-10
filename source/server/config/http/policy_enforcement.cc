#include "server/config/http/policy_enforcement.h"

#include <string>

#include "common/http/filter/policy_enforcement_filter.h"
#include "common/json/config_schemas.h"
#include "server/config/network/http_connection_manager.h"

namespace Envoy {
namespace Server {
namespace Configuration {

HttpFilterFactoryCb PolicyEnforcementFilterConfig::createFilterFactory(const Json::Object& json_config,
                                                           const std::string& stats_prefix,
                                                           FactoryContext& context) {
  ENVOY_LOG(warn, "PolicyEnforcementFilterConfig::createFilterFactory called");
  Http::PolicyEncorcementFilterConfigSharedPtr config(
      new Http::PolicyEnforcementFilterConfig(json_config, context.runtime(), stats_prefix, context.scope()));
  return [config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
    callbacks.addStreamDecoderFilter(
        Http::StreamDecoderFilterSharedPtr{new Http::PolicyEnforcementFilter(config)});
  };
}

std::string name() override { return "policy_enforcement"; }
HttpFilterType type() override { return HttpFilterType::Decoder; }

/**
 * Static registration for the fault filter. @see RegisterFactory.
 */
static Registry::RegisterFactory<PolicyEnforcementFilterConfig, NamedHttpFilterConfigFactory> register_;

} // Configuration
} // Server
} // Envoy
