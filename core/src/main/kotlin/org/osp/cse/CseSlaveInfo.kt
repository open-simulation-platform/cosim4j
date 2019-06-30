package org.osp.cse


class CseSlaveInfo {

    lateinit var name: String
        private set
    lateinit var source: String
        private set

    var index: Int = -1
        private set

    override fun toString(): String {
        return "CseSlaveInfo(name=$name, source=$source, index=$index)"
    }

}
