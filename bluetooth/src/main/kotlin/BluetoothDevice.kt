/*
 * Copyright (c) 2026. Umamusume Polska
 */

data class DeviceInfo(val address: String, val name: String)

class BluetoothDevice internal constructor(
    private val ptr: Long,
    val info: DeviceInfo
) : AutoCloseable {
    val address: String get() = info.address
    val name: String get() = info.name

    private val cleanable = cleaner.register(this, NativeDeallocator(ptr))

    companion object {
        private val cleaner = java.lang.ref.Cleaner.create()

        private external fun nativeCreateSocket(ptr: Long, channel: Int): Long

        private external fun nativeRelease(ptr: Long)
    }

    private class NativeDeallocator(private val ptr: Long) : Runnable {
        override fun run() {
            nativeRelease(ptr)
        }
    }

    fun createSocket(channel: Int): RfcommSocket {
        val socketPtr = nativeCreateSocket(ptr, channel)
        return RfcommSocket(socketPtr)
    }

    override fun close() {
        cleanable.clean()
    }

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (other !is BluetoothDevice) return false
        return address == other.address
    }

    override fun hashCode(): Int = address.hashCode()

    override fun toString(): String = "BluetoothDevice(address='$address', name='$name')"
}