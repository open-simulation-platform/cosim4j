@file: JvmName("SSPUtil")

package org.osp.util

import java.io.File

const val SSP_FILE_NAME = "SystemStructure.ssd"

fun extractStepSize(sspDir: File): Double? {

    val sspFile = File(sspDir, SSP_FILE_NAME)
    if (!sspFile.exists()) {
        throw NoSuchFileException(sspFile)
    }

   return extractStepSize(sspFile.readText())

}

internal fun extractStepSize(text: String): Double? {

    val regex = "stepSize=\"(\\d+\\.\\d+)\"".toRegex()
    return regex.findAll(text).toList().map { it.groupValues }.getOrNull(0)?.getOrNull(1)?.toDouble()

}
