#!/usr/bin/env bash

conan remote add osp https://osp-conan.azurewebsites.net/artifactory/api/conan/conan-local --f
conan user -p "Open Simulation Platform" -r osp osp

unameOut="$(uname -s)"
case "${unameOut}" in
    Linux*)     conan install . -s build_type=Release -s compiler.libcxx=libstdc++11 --install-folder=cmake-build-release --build=missing
                conan install . -s build_type=Debug -s compiler.libcxx=libstdc++11 --install-folder=cmake-build-debug --build=missing;;
    MINGW*)     conan install . -s build_type=Debug --install-folder=cmake-build-debug --build=missing
                conan install . -s build_type=Release --install-folder=cmake-build-release --build=missing;;
esac
