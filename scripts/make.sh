#!/bin/bash

script="$(readlink -f $0)"
script_dir="$(dirname $script)"

build_dir="$script_dir/buildx86"
install_dir="$(dirname $script_dir)/installx86"


cd $build_dir 
make
#make install
