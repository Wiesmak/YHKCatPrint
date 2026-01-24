/*
 * Copyright (c) 2026. Umamusume Polska
 */

package pl.umamusume.yhkcatprint.bluetooth

import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext

data class AdapterInfo(val address: String, val name: String)

class BluetoothAdapter internal constructor(
    private val ptr: Long,
    val info: AdapterInfo,
    private val gateway: BluetoothNativeGateway
) : AutoCloseable {
    val address: String get() = info.address
    val name: String get() = info.name

    private val adapterGateway = gateway.bluetoothAdapter
    private val cleanable = NativeCleaner.register(
        this,NativeDeallocator(ptr, adapterGateway)
    )

    private suspend fun getDeviceByPtr(devicePtr: Long): BluetoothDevice = withContext(Dispatchers.IO) {
        val info = DeviceInfo(
            address = adapterGateway.getDeviceAddress(devicePtr),
            name = adapterGateway.getDeviceName(devicePtr)
        )
        BluetoothDevice(devicePtr, info, gateway)
    }

    suspend fun getPairedDevices(): List<BluetoothDevice> = withContext(Dispatchers.IO) {
        val pointers = adapterGateway.getPaired(ptr)
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