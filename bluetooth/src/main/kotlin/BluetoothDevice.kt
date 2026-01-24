/*
 * Copyright (c) 2026. Umamusume Polska
 */

data class DeviceInfo(val address: String, val name: String)

class BluetoothDevice internal constructor(
    private val ptr: Long,
    val info: DeviceInfo,
    private val gateway: BluetoothNativeGateway
) : AutoCloseable {
    val address: String get() = info.address
    val name: String get() = info.name

    private val deviceGateway = gateway.bluetoothDevice
    private val cleanable = NativeCleaner.register(
        this, NativeDeallocator(ptr, deviceGateway)
    )

    fun createSocket(channel: Int): RfcommSocket {
        val socketPtr = deviceGateway.createSocket(ptr, channel)
        return RfcommSocket(socketPtr, gateway)
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