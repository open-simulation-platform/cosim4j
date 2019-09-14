package org.osp.cse

import java.lang.IllegalArgumentException

enum class CseLogLevel(
        val code: Int
) {

    CSE_LOG_SEVERITY_TRACE(0),
    CSE_LOG_SEVERITY_DEBUG(1),
    CSE_LOG_SEVERITY_INFO(2),
    CSE_LOG_SEVERITY_WARNING(3),
    CSE_LOG_SEVERITY_ERROR(4),
    CSE_LOG_SEVERITY_FATAL(5);

    companion object {

        @JvmStatic
        fun valueOf(i: Int): CseLogLevel {
            for (logLevel in values()) {
                if (i == logLevel.code) {
                    return logLevel
                }
            }
            throw IllegalArgumentException("$i not in range of ${values().map { it.code }}")
        }

    }

}
