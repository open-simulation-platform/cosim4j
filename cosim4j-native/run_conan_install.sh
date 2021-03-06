#!/usr/bin/env bash

conan remote add osp https://osp.jfrog.io/artifactory/api/conan/conan-local --f
export CONAN_REVISIONS_ENABLED="1"

unameOut="$(uname -s)"
case "${unameOut}" in
    MINGW*)     conan install . -s build_type=Release --install-folder=cmake-build-release --build=missing;;
    Linux*)     conan install . -s build_type=Release -s compiler.libcxx=libstdc++11 --install-folder=cmake-build-release --build=missing;;
esac
