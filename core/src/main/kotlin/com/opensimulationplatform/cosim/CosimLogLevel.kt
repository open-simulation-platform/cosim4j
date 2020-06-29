package com.opensimulationplatform.cosim

enum class CosimLogLevel(
        val code: Int
) {

    COSIM_LOG_SEVERITY_TRACE(0),
    COSIM_LOG_SEVERITY_DEBUG(1),
    COSIM_LOG_SEVERITY_INFO(2),
    COSIM_LOG_SEVERITY_WARNING(3),
    COSIM_LOG_SEVERITY_ERROR(4),
    COSIM_LOG_SEVERITY_FATAL(5);

    companion object {

        @JvmStatic
        fun valueOf(i: Int): CosimLogLevel {
            for (logLevel in values()) {
                if (i == logLevel.code) {
                    return logLevel
                }
            }
            throw IllegalArgumentException("$i not in range of ${values().map { it.code }}")
        }

    }

}
