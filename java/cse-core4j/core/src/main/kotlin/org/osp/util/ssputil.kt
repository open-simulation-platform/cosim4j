@file: JvmName("SSPUtil")

package org.osp.util

import java.io.File

const val SSP_FILE_NAME = "SystemStructure.ssd"

fun extractFixedStepAlgorithmStepSize(sspDir: File): Double? {

    val sspFile = File(sspDir, SSP_FILE_NAME)
    if (!sspFile.exists()) {
        throw NoSuchFileException(sspFile)
    }

    return extractFixedStepAlgorithmStepSize(sspFile.readText())

}

internal fun extractFixedStepAlgorithmStepSize(text: String): Double? {

    val regex = "osp:FixedStepAlgorithm baseStepSize=\"(\\d+\\.\\d+)?(\\d+e-\\d+)?\"".toRegex()
    val groups = regex.findAll(text).toList().map { it.groupValues }
    return groups.getOrNull(0)?.let { match ->

        val normalNotation = match[1]
        val scientificNotation = match[2]
        when {
            normalNotation.isNotEmpty() -> normalNotation.toDouble()
            scientificNotation.isNotEmpty() -> scientificNotation.toDouble()
            else -> null
        }
    }

}
