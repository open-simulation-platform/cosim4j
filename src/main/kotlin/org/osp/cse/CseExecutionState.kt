package org.osp.cse

import java.lang.IllegalArgumentException

enum class CseExecutionState(
    val code: Int
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