# cosim4j

[![CI](https://github.com/open-simulation-platform/cosim4j/workflows/CI/badge.svg)](https://github.com/open-simulation-platform/cosim4j/actions)

High-level, easy to use Java bindings for [libcosim](https://github.com/open-simulation-platform/libcosim), written in Kotlin.

The JNI code is written by hand in clean, easy to maintain C++, no magic involved. 

#### Usage with maven

Add the open-simulation-platform bintray repository:

```gradle
repositories {ntnu-ih
    maven { url  "https://dl.bintray.com/open-simulation-platform/maven"  }
}
```

Add the dependencies

```gradle
implementation "org.osp.cosim4j:core:r1"

// one of
runtimeOnly "org.osp.cosim4j:natives-win:r1"
runtimeOnly "org.osp.cosim4j:natives-linux:r1"
```


#### Building from source

1) Fetch and build native dependencies by running `gradlew buildNative`. This is only required if you have modified `conanfile.txt` or the JNI sources.
2) Build and test cosim4j by running `gradlew build`


