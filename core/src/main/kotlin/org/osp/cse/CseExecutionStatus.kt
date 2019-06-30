package org.osp.cse


class CseExecutionStatus {

    var currentTime: Double = -1.0
    private set

    var realTimeFactor: Double = 0.0
    private set

    var realTimeSimulation: Boolean = false
    private set

    private var stateId: Int = -1

    val state: CseExecutionState by lazy {
        CseExecutionState.valueOf(stateId)
    }

    private var errorCodeId: Int = -1

    val errorCode: CseErrorCode by lazy {
        CseErrorCode.valueOf(errorCodeId)
    }

    override fun toString(): String {
        return "CseExecutionStatus(currentTime=$currentTime, state=$state, errorCode=$errorCode, realTimeFactor=$realTimeFactor, isRealTimeSimulation=$realTimeSimulation)"
    }

}
