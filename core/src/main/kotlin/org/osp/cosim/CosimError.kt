package org.osp.cosim

import java.lang.IllegalArgumentException

class CosimError(
        private val code: Int,
        val message: String
) {

    val error: CosimErrorCode by lazy {
        CosimErrorCode.valueOf(code)
    }

    override fun toString(): String {
        return "CosimError(error=$error, message='$message')"
    }

}


enum class CosimErrorCode(
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

    ILLEGAL_STATE(4),

    /**
     * Index out of range.
     */
    OUT_OF_RANGE(5),

    /**
     *  The time step failed, but can be retried with a shorter step length
     *  (if supported by all slaves).
     */
    STEP_TOO_LONG(6),

    /**
     * An input file is corrupted or invalid.
     */
    BAD_FILE(7),

    /**
     * The requested feature (e.g. an FMI feature) is unsupported.
     */
    UNSUPPORTED_FEATURE(8),

    /**
     * Error loading dynamic library (e.g. model code).
     */
    DL_LOAD_ERROR(9),

    /**
     * The model reported an error.
     */
    MODEL_ERROR(10),

    /**
     * ZIP file error.
     */
    ZIP_ERROR(11);


    companion object {

        @JvmStatic
        fun valueOf(i: Int): CosimErrorCode {
            for (state in values()) {
                if (i == state.code) {
                    return state
                }
            }
            throw IllegalArgumentException("$i not in range of ${values().map { it.code }}")
        }

    }

}
