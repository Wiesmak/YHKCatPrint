/*
 * Copyright (c) 2026. Umamusume Polska
 */

import javax.security.auth.callback.Callback

interface BluetoothNativeGateway {
    val bluetoothManager: BluetoothManagerGateway
    val bluetoothAdapter: BluetoothAdapterGateway
    val bluetoothDevice: BluetoothDeviceGateway
    val rfcommSocket: RfcommSocketGateway

    interface NativeCleaner {
        fun release(ptr: Long)
    }

    interface BluetoothManagerGateway : NativeCleaner {
        fun init(): Long
        fun listAdapters(ptr: Long): LongArray
        fun getAdapterAddress(ptr: Long): String
        fun getAdapterName(ptr: Long): String
        fun getAdapter(ptr: Long, address: String): Long
        override fun release(ptr: Long)
    }

    interface BluetoothAdapterGateway : NativeCleaner {
        fun getPaired(ptr: Long): LongArray
        fun getDeviceAddress(ptr: Long): String
        fun getDeviceName(ptr: Long): String
        override fun release(ptr: Long)
    }

    interface BluetoothDeviceGateway : NativeCleaner {
        fun createSocket(ptr: Long, channel: Int): Long
        override fun release(ptr: Long)
    }

    interface RfcommSocketGateway : NativeCleaner {
        fun connect(ptr: Long, timeoutMs: Long, callback: Callback)
        fun cancelConnect(ptr: Long)
        fun startReading(ptr: Long, callback: Callback)
        fun stopReading(ptr: Long)
        fun write(ptr: Long, data: ByteArray)
        fun close(ptr: Long)
        override fun release(ptr: Long)
    }
}