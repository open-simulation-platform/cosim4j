# cosim4j

[![CI](https://github.com/open-simulation-platform/cosim4j/workflows/CI/badge.svg)](https://github.com/open-simulation-platform/cosim4j/actions)

High-level, easy to use Java bindings for [libcosim](https://github.com/open-simulation-platform/libcosim), written in Kotlin.

The JNI code is written by hand in clean, easy to maintain C++, no magic involved. 

#### Building from source

1) Fetch and build native dependencies by running `gradlew buildNative`. This is only required if you have modified `conanfile.txt` or the JNI sources.
2) Build and test cosim4j by running `gradlew build`


