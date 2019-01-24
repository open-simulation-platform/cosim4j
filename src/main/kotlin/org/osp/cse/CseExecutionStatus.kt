package org.osp.cse

interface CseExecutionStatus {

    val currentTime: Double
    val state: CseExecutionState
    val errorCode: CseErrorCode
    val realTimeFactor: Double
    val realTimeSimulation: Boolean

}

class CseExecutionStatusImpl: CseExecutionStatus {

    private var errorCodeId: Int = -1

    override val errorCode: CseErrorCode by lazy {
        CseErrorCode.valueOf(errorCodeId)
    }

    override var currentTime: Double = -1.0

    override var realTimeFactor: Double = 0.0

    override var realTimeSimulation: Boolean = false

    private var stateId: Int = -1

    override val state: CseExecutionState by lazy {
        CseExecutionState.valueOf(stateId)
    }

    override fun toString(): String {
        return "CseExecutionStatusImpl(currentTime=$currentTime, state=$state, errorCode=$errorCode, realTimeFactor=$realTimeFactor, isRealTimeSimulation=$realTimeSimulation)"
    }

}