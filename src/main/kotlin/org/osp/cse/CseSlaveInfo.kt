package org.osp.cse

import java.lang.IllegalStateException

class CseSlaveInfo {
    private var name_: String? = null
    val name: String by lazy {
        name_ ?: throw IllegalStateException("Not initialized yet!")
    }

    private var source_: String? = null
    val source: String by lazy {
        source_ ?: throw IllegalStateException("Not initialized yet!")
    }

    var index: Int = -1
    private set

    override fun toString(): String {
        return "CseSlaveInfo(name=$name, source=$source, index=$index)"
    }

}