#!/usr/bin/env bash

conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan --f
conan remote add osp https://osp-conan.azurewebsites.net/artifactory/api/conan/conan-local --f
conan user -p "Open Simulation Platform" -r osp osp

unameOut="$(uname -s)"
case "${unameOut}" in
    MINGW*)     conan install . -s build_type=Release --install-folder=cmake-build-release --build=missing;;
    Linux*)     conan install . -s build_type=Release -s compiler.libcxx=libstdc++11 --install-folder=cmake-build-release --build=missing;;
esac
