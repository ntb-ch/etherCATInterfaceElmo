#!/bin/bash

script="$(readlink -f $0)"
script_dir="$(dirname $script)"

build_dir="$(dirname "$script_dir")/buildx86"
install_dir="~/installx86"


rm -rf "$build_dir"
echo rm -rf "$build_dir"
		

mkdir -p $build_dir
pushd $build_dir
pwd
echo $build_dir
echo cmake -DCMAKE_INSTALL_PREFIX="$install_dir" ..
cmake -DCMAKE_BUILD_TYPE="RelWithDebInfo" -DCMAKE_INSTALL_PREFIX="$install_dir" -g ..
#cmake -DCMAKE_BUILD_TYPE="RelWithDebInfo" -DCMAKE_INSTALL_PREFIX="$install_dir" -g -DUSE_ETHERCAT=TRUE ..
make
make install
popd
