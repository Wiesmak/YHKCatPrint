/*
 * Copyright (c) 2026. Umamusume Polska
 */

import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext

data class AdapterInfo(val address: String, val name: String)

class BluetoothAdapter internal constructor(
    private val ptr: Long,
    val info: AdapterInfo
) : AutoCloseable {
    val address: String get() = info.address
    val name: String get() = info.name

    private val cleanable = NativeCleaner.register(this, NativeDeallocator(ptr))

    companion object {
        private external fun nativeGetPaired(ptr: Long): LongArray

        private external fun nativeGetDeviceAddress(ptr: Long): String

        private external fun nativeGetDeviceName(ptr: Long): String

        private external fun nativeRelease(ptr: Long)
    }

    private class NativeDeallocator(private val ptr: Long) : Runnable {
        override fun run() {
            nativeRelease(ptr)
        }
    }

    private suspend fun getDeviceByPtr(devicePtr: Long): BluetoothDevice = withContext(Dispatchers.IO) {
        val info = DeviceInfo(
            address = nativeGetDeviceAddress(devicePtr),
            name = nativeGetDeviceName(devicePtr)
        )
        BluetoothDevice(devicePtr, info)
    }

    suspend fun getPairedDevices(): List<BluetoothDevice> = withContext(Dispatchers.IO) {
        val pointers = nativeGetPaired(ptr)
        pointers.map { getDeviceByPtr(it) }
    }

    override fun close() {
        cleanable.clean()
    }

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (other !is BluetoothAdapter) return false
        return address == other.address
    }

    override fun hashCode(): Int = address.hashCode()

    override fun toString(): String = "BluetoothAdapter(address='$address', name='$name')"
}