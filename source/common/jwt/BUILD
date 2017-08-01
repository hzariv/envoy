licenses(["notice"])  # Apache 2

load(
    "//bazel:envoy_build_system.bzl",
    "envoy_cc_library",
    "envoy_package",
)

envoy_package()

envoy_cc_library(
    name = "jwt_lib",
    srcs = ["jwt.cc"],
    hdrs = ["jwt.h"],
    deps = [
            "//source/common/jwt:base64_lib",
            "//source/common/jwt:claimvalidator_lib",
            "//source/common/jwt:claimvalidatorfactory_lib",
            "//source/common/jwt:hmacvalidator_lib",
            "//source/common/jwt:kidvalidator_lib",
            "//source/common/jwt:listclaimvalidator_lib",
            "//source/common/jwt:messagevalidator_lib",
            "//source/common/jwt:messagevalidatorfactory_lib",
            "//source/common/jwt:nonevalidator_lib",
            "//source/common/jwt:rsavalidator_lib",
            "//source/common/jwt:setvalidator_lib",
            "//source/common/jwt:timevalidator_lib",
            "//source/common/jansson:jansson_lib",
        ],
)

envoy_cc_library(
    name = "base64_lib",
    srcs = ["base64.cc"],
    hdrs = ["base64.h"],
      deps = [
              "//source/common/jansson:jansson_lib",
         ],
)

envoy_cc_library(
    name = "allocators_lib",
    hdrs = ["allocators.h"],
    deps = [
        "//source/common/jansson:jansson_lib",
    ],
)

envoy_cc_library(
    name = "claimvalidator_lib",
    srcs = ["claimvalidator.cc"],
    hdrs = ["claimvalidator.h"],
    deps = [
          "//source/common/jansson:jansson_lib",
     ],
)

envoy_cc_library(
    name = "claimvalidatorfactory_lib",
    srcs = ["claimvalidatorfactory.cc"],
    hdrs = ["claimvalidatorfactory.h"],
     deps = [
             "//source/common/jansson:jansson_lib",
        ],
)

envoy_cc_library(
    name = "hmacvalidator_lib",
    srcs = ["hmacvalidator.cc"],
    hdrs = glob(["thirdparty_build/include/openssl/**/*.h"]) + 
            ["hmacvalidator.h"],
    deps = [
             "//source/common/jansson:jansson_lib",
        ],
)

envoy_cc_library(
    name = "kidvalidator_lib",
    srcs = ["kidvalidator.cc"],
    hdrs = ["kidvalidator.h"],
      deps = [
              "//source/common/jansson:jansson_lib",
         ],
)

envoy_cc_library(
    name = "listclaimvalidator_lib",
    srcs = ["listclaimvalidator.cc"],
    hdrs = ["listclaimvalidator.h"],
      deps = [
              "//source/common/jansson:jansson_lib",
         ],
)

envoy_cc_library(
    name = "messagevalidator_lib",
    srcs = ["messagevalidator.cc"],
    hdrs = ["messagevalidator.h"],
    deps = [
         "//source/common/jansson:jansson_lib",
    ],
)

envoy_cc_library(
    name = "messagevalidatorfactory_lib",
    srcs = ["messagevalidatorfactory.cc"],
    hdrs = ["messagevalidatorfactory.h"],
      deps = [
                 "//source/common/jansson:jansson_lib",
            ],
)

envoy_cc_library(
    name = "nonevalidator_lib",
    srcs = ["nonevalidator.cc"],
    hdrs = ["nonevalidator.h"],
    deps = [
              "//source/common/jansson:jansson_lib",
         ],
)

envoy_cc_library(
    name = "rsavalidator_lib",
    srcs = ["rsavalidator.cc"],
    hdrs = glob(["thirdparty_build/include/openssl/**/*.h"]) + 
        ["rsavalidator.h"],
      deps = [
              "//source/common/jansson:jansson_lib",
         ],
)

envoy_cc_library(
    name = "setvalidator_lib",
    srcs = ["setvalidator.cc"],
    hdrs = ["setvalidator.h"],
    deps = [
             "//source/common/jansson:jansson_lib",
        ],
)

envoy_cc_library(
    name = "timevalidator_lib",
    srcs = ["timevalidator.cc"],
    hdrs = ["timevalidator.h"],
    deps = [
          "//source/common/jansson:jansson_lib",
     ],
)

