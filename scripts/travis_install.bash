#!/bin/bash

# Install gcc 7
sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-get update -qq
sudo apt-get install -qq g++-7
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-7 90

## Install CMake 3.13.2
DEPS_DIR="${TRAVIS_BUILD_DIR}/deps"
mkdir ${DEPS_DIR}
cd ${DEPS_DIR}
travis_retry wget https://github.com/Kitware/CMake/releases/download/v3.13.2/cmake-3.13.2-Linux-x86_64.tar.gz
echo "6370de82999baafc2dbbf0eda23007d93f78d0c3afda8434a646518915ca0846  cmake-3.13.2-Linux-x86_64.tar.gz" > cmake_sha256.txt
sha256sum -c cmake_sha256.txt
tar -xvf cmake-3.13.2-Linux-x86_64.tar.gz > /dev/null
mv cmake-3.13.2-Linux-x86_64 cmake-install
PATH=${DEPS_DIR}/cmake-install:${DEPS_DIR}/cmake-install/bin:$PATH
cd ${TRAVIS_BUILD_DIR}
