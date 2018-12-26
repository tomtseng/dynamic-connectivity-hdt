#!/bin/bash

base_dir=$(git rev-parse --show-toplevel)
build_dir=${base_dir}/build

mkdir ${build_dir}
cd ${build_dir}
cmake ${base_dir}
make
