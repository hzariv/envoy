#!/bin/bash

set -e

git clone https://github.com/pokowaka/jwt-cpp
cd jwt-cpp

cp -r /usr/local/include/jwt/* $THIRDPARTY_BUILD/include
cp /usr/local/lib/libjwt.a $THIRDPARTY_BUILD/lib
cp crypto/libcrypto.a $THIRDPARTY_BUILD/lib
