package org.osp.cse

import java.lang.IllegalArgumentException

enum class CseVariableType(
        private val code: Int
) {

    CSE_VARIABLE_TYPE_REAL(0),
    CSE_VARIABLE_TYPE_INTEGER(1),
    CSE_VARIABLE_TYPE_STRING(2),
    CSE_VARIABLE_TYPE_BOOLEAN(3);

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

    CSE_VARIABLE_CAUSALITY_INPUT(0),
    CSE_VARIABLE_CAUSALITY_PARAMETER(1),
    CSE_VARIABLE_CAUSALITY_OUTPUT(2),
    CSE_VARIABLE_CAUSALITY_CALCULATEDPARAMETER(3),
    CSE_VARIABLE_CAUSALITY_LOCAL(4),
    CSE_VARIABLE_CAUSALITY_INDEPENDENT(5);

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

    CSE_VARIABLE_VARIABILITY_CONSTANT(0),
    CSE_VARIABLE_VARIABILITY_FIXED(1),
    CSE_VARIABLE_VARIABILITY_TUNABLE(2),
    CSE_VARIABLE_VARIABILITY_DISCRETE(3),
    CSE_VARIABLE_VARIABILITY_CONTINUOUS(4);

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
    CSE_EXECUTION_UNKNOWN(-1),
    CSE_EXECUTION_STOPPED(0),
    CSE_EXECUTION_RUNNING(1),
    CSE_EXECUTION_ERROR(2);

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
    CSE_ERRC_NONE(-1),

    /**
     * All OK
     */
    CSE_ERRC_SUCCESS(0),

    /**
     * Unspecified error (but message may contain details).
     */
    CSE_ERRC_UNSPECIFIED(1),

    /**
     * Error reported by C/C++ runtime; check `errno` to get the right code.
     */
    CSE_ERRC_ERRNO(2),

    /**
     * Invalid function argument.
     */
    CSE_ERRC_INVALID_ARGUMENT(3),

    /**
     * Index out of range.
     */
    CSE_ERRC_OUT_OF_RANGE(4),

    /**
     *  The time step failed, but can be retried with a shorter step length
     *  (if supported by all slaves).
     */
    CSE_ERRC_STEP_TOO_LONG(5),

    /**
     * An input file is corrupted or invalid.
     */
    CSE_ERRC_BAD_FILE(6),

    /**
     * The requested feature (e.g. an FMI feature) is unsupported.
     */
    CSE_ERRC_UNSUPPORTED_FEATURE(7),

    /**
     * Error loading dynamic library (e.g. model code).
     */
    CSE_ERRC_DL_LOAD_ERROR(8),

    /**
     * The model reported an error.
     */
    CSE_ERRC_MODEL_ERROR(9),

    /**
     * ZIP file error.
     */
    CSE_ERRC_ZIP_ERROR(10);


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
