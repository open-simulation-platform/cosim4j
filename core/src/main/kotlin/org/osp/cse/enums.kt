package org.osp.cse

import java.lang.IllegalArgumentException

enum class CseVariableType(
        private val code: Int
) {

    REAL(0),
    INTEGER(1),
    STRING(2),
    BOOLEAN(3);

    companion object {

        @JvmStatic
        fun valueOf(i: Int): CseVariableType {
            for (state in values()) {
                if (i == state.code) {
                    return state
                }
            }
            throw IllegalArgumentException("$i not in range of ${values().map { it.code }}")
        }

    }

}

enum class CseVariableCausality(
        private val code: Int
) {

    INPUT(0),
    PARAMETER(1),
    OUTPUT(2),
    CALCULATED_PARAMETER(3),
    LOCAL(4),
    INDEPENDENT(5);

    companion object {

        @JvmStatic
        fun valueOf(i: Int): CseVariableCausality {
            for (state in values()) {
                if (i == state.code) {
                    return state
                }
            }
            throw IllegalArgumentException("$i not in range of ${values().map { it.code }}")
        }

    }

}

enum class CseVariableVariability(
        private val code: Int
) {

    CONSTANT(0),
    FIXED(1),
    TUNABLE(2),
    DISCRETE(3),
    CONTINUOUS(4);

    companion object {

        @JvmStatic
        fun valueOf(i: Int): CseVariableVariability {
            for (state in values()) {
                if (i == state.code) {
                    return state
                }
            }
            throw IllegalArgumentException("$i not in range of ${values().map { it.code }}")
        }

    }
}

enum class CseExecutionState(
        private val code: Int
) {
    UNKNOWN(-1),
    STOPPED(0),
    RUNNING(1),
    ERROR(2);

    companion object {

        @JvmStatic
        fun valueOf(i: Int): CseExecutionState {
            for (state in values()) {
                if (i == state.code) {
                    return state
                }
            }
            throw IllegalArgumentException("$i not in range of ${values().map { it.code }}")
        }

    }

}

enum class CseErrorCode(
        private val code: Int
) {

    /**
     * Undefined
     */
    NONE(-1),

    /**
     * All OK
     */
    SUCCESS(0),

    /**
     * Unspecified error (but message may contain details).
     */
    UNSPECIFIED(1),

    /**
     * Error reported by C/C++ runtime; check `errno` to get the right code.
     */
    ERRNO(2),

    /**
     * Invalid function argument.
     */
    INVALID_ARGUMENT(3),

    /**
     * Index out of range.
     */
    OUT_OF_RANGE(4),

    /**
     *  The time step failed, but can be retried with a shorter step length
     *  (if supported by all slaves).
     */
    STEP_TOO_LONG(5),

    /**
     * An input file is corrupted or invalid.
     */
    BAD_FILE(6),

    /**
     * The requested feature (e.g. an FMI feature) is unsupported.
     */
    UNSUPPORTED_FEATURE(7),

    /**
     * Error loading dynamic library (e.g. model code).
     */
    DL_LOAD_ERROR(8),

    /**
     * The model reported an error.
     */
    MODEL_ERROR(9),

    /**
     * ZIP file error.
     */
    ZIP_ERROR(10);


    companion object {

        @JvmStatic
        fun valueOf(i: Int): CseErrorCode {
            for (state in values()) {
                if (i == state.code) {
                    return state
                }
            }
            throw IllegalArgumentException("$i not in range of ${values().map { it.code }}")
        }

    }

}

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
