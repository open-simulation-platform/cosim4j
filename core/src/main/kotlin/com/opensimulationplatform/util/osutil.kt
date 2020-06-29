package com.opensimulationplatform.util

internal val osName: String = System.getProperty("os.name")
internal val platformBitness: String = System.getProperty("sun.arch.data.model")

internal val is32Bit: Boolean
    get() = platformBitness == "32"

internal val is64Bit: Boolean
    get() = platformBitness == "64"

internal val isWindows: Boolean
    get() = osName.startsWith("Windows")

internal val isLinux: Boolean
    get() = osName.startsWith("Linux")

internal val isMac: Boolean
    get() = osName.startsWith("Mac") || osName.startsWith("Darwin")

internal val currentOS: String
    get() {
        return when {
            isMac -> "darwin$platformBitness"
            isLinux -> "linux$platformBitness"
            isWindows -> "win$platformBitness"
            else -> throw RuntimeException("Unsupported OS: $osName")
        }
    }

internal val libPrefix: String
    get() {
        return when {
            isMac -> "" // NOT SURE IF THIS IS CORRECT!
            isLinux -> "lib"
            isWindows -> ""
            else -> throw RuntimeException("Unsupported OS: $osName")
        }
    }

internal val sharedLibExtension: String
    get() {
        return when {
            isMac -> "dylib"
            isLinux -> "so"
            isWindows -> "dll"
            else -> throw RuntimeException("Unsupported OS: $osName")
        }
    }
