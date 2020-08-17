# cosim4j

[![CI](https://github.com/open-simulation-platform/cosim4j/workflows/CI/badge.svg)](https://github.com/open-simulation-platform/cosim4j/actions)
[ ![Download](https://api.bintray.com/packages/open-simulation-platform/maven/cosim4j/images/download.svg) ](https://bintray.com/open-simulation-platform/maven/cosim4j/_latestVersion)

High-level, easy to use JVM bindings for [libcosim](https://github.com/open-simulation-platform/libcosim), written in Kotlin.

The JNI code is written by hand in clean, easy to maintain C++, no magic involved. 

#### Usage with maven

Add the open-simulation-platform bintray repository:

```gradle
repositories {
    maven { url  "https://dl.bintray.com/open-simulation-platform/maven" }
    maven { url  "https://dl.bintray.com/ntnu-ihb/mvn" } // <- needed to resolve fmi4j module used to create JVM slaves
}
```

Add the dependencies

```gradle
def cosim4j_version = "..."
implementation "com.opensimulationplatform.cosim4j:core:$cosim4j_version"

// one of
runtimeOnly "com.opensimulationplatform.cosim4j:natives-win:$cosim4j_version"
runtimeOnly "com.opensimulationplatform.cosim4j:natives-linux:$cosim4j_version"
```


#### Building from source

1) Fetch and build native dependencies by running `gradlew buildNative`. This is only required if you have modified `conanfile.txt` or the JNI sources.
2) Build and test cosim4j by running `gradlew build`


