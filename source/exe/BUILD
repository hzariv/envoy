licenses(["notice"])  # Apache 2

load(
    "//bazel:envoy_build_system.bzl",
    "envoy_cc_binary",
    "envoy_cc_library",
    "envoy_package",
)

envoy_package()

alias(
    name = "envoy",
    actual = ":envoy-static",
)

envoy_cc_binary(
    name = "envoy-static",
    stamped = True,
    deps = ["envoy_main_entry_lib"],
)

envoy_cc_library(
    name = "envoy_common_lib",
    deps = [
        "//source/common/event:libevent_lib",
        "//source/common/local_info:local_info_lib",
        "//source/common/network:utility_lib",
        "//source/common/stats:stats_lib",
        "//source/common/stats:thread_local_store_lib",
        "//source/server:drain_manager_lib",
        "//source/server:options_lib",
        "//source/server:server_lib",
        "//source/server:test_hooks_lib",
        "//source/server/config/http:buffer_lib",
        "//source/server/config/http:dynamo_lib",
        "//source/server/config/http:fault_lib",
        "//source/server/config/http:grpc_http1_bridge_lib",
        "//source/server/config/http:grpc_web_lib",
        "//source/server/config/http:ratelimit_lib",
        "//source/server/config/http:router_lib",
        "//source/server/config/network:client_ssl_auth_lib",
        "//source/server/config/network:echo_lib",
        "//source/server/config/network:http_connection_manager_lib",
        "//source/server/config/network:mongo_proxy_lib",
        "//source/server/config/network:ratelimit_lib",
        "//source/server/config/network:redis_proxy_lib",
        "//source/server/config/network:tcp_proxy_lib",
        "//source/server/http:health_check_lib",
        "//source/server/config/http:policy_enforcement_lib",
    ],
)

envoy_cc_library(
    name = "envoy_main_entry_lib",
    srcs = ["main.cc"],
    deps = [
        ":envoy_main_common_lib",
        ":hot_restart_lib",
        "//source/server:options_lib",
        "//source/server/config/http:lightstep_lib",
        "//source/server/config/http:zipkin_lib",
    ] + select({
        "//bazel:disable_signal_trace": [],
        "//conditions:default": [":sigaction_lib"],
    }),
)

envoy_cc_library(
    name = "envoy_main_common_lib",
    srcs = ["main_common.cc"],
    hdrs = ["main_common.h"],
    deps = [
        ":envoy_common_lib",
        ":hot_restart_lib",
        "//source/common/common:compiler_requirements_lib",
        "//source/server/config_validation:server_lib",
    ],
)

envoy_cc_library(
    name = "hot_restart_lib",
    srcs = ["hot_restart.cc"],
    hdrs = ["hot_restart.h"],
    deps = [
        "//include/envoy/event:dispatcher_interface",
        "//include/envoy/event:file_event_interface",
        "//include/envoy/server:hot_restart_interface",
        "//include/envoy/server:instance_interface",
        "//include/envoy/server:options_interface",
        "//source/common/common:assert_lib",
        "//source/common/common:utility_lib",
        "//source/common/network:utility_lib",
        "//source/common/stats:stats_lib",
    ],
)

envoy_cc_library(
    name = "sigaction_lib",
    srcs = ["signal_action.cc"],
    hdrs = ["signal_action.h"],
    tags = ["backtrace"],
    deps = [
        "//source/common/common:assert_lib",
        "//source/common/common:non_copyable",
        "//source/server:backtrace_lib",
    ],
)
