#!/usr/bin/env bash

unameOut="$(uname -s)"
case "${unameOut}" in
    Linux*)     conan install . -s build_type=Release -s compiler.libcxx=libstdc++11 --install-folder=build --build=missing;;
    MINGW*)     conan install . -s build_type=Release --install-folder=build --build=missing;;
esac
