package org.osp.cse

enum class CseErrorCode(
    val code: Int
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