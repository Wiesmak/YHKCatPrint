/*
 * Copyright (c) 2026. Umamusume Polska
 */

package pl.umamusume.yhkcatprint.bluetooth

import javax.security.auth.callback.Callback

object Win32NativeGateway : BluetoothNativeGateway {
    init {
        // TODO: Change the path to a relative one
        System.load("D:/IdeaProjects/YHKCatPrint/bluetooth/libs/YHKCatPrintNative/x64/Release/YHKCatPrint.dll")
    }

    override val bluetoothManager: BluetoothNativeGateway.BluetoothManagerGateway get() = BluetoothManager
    override val bluetoothAdapter: BluetoothNativeGateway.BluetoothAdapterGateway get() = BluetoothAdapter
    override val bluetoothDevice: BluetoothNativeGateway.BluetoothDeviceGateway get() = BluetoothDevice
    override val rfcommSocket: BluetoothNativeGateway.RfcommSocketGateway get() = RfcommSocket

    object BluetoothManager : BluetoothNativeGateway.BluetoothManagerGateway {
        init { Win32NativeGateway.hashCode() }
        external override fun init(): Long
        external override fun listAdapters(ptr: Long): LongArray
        external override fun getAdapterAddress(ptr: Long): String
        external override fun getAdapterName(ptr: Long): String
        external override fun getAdapter(ptr: Long, address: String): Long
        external override fun release(ptr: Long)
    }

    object BluetoothAdapter : BluetoothNativeGateway.BluetoothAdapterGateway {
        init { Win32NativeGateway.hashCode() }
        external override fun getPaired(ptr: Long): LongArray
        external override fun getDeviceAddress(ptr: Long): String
        external override fun getDeviceName(ptr: Long): String
        external override fun release(ptr: Long)
    }

    object BluetoothDevice : BluetoothNativeGateway.BluetoothDeviceGateway {
        init { Win32NativeGateway.hashCode() }
        external override fun createSocket(ptr: Long, channel: Int): Long
        external override fun release(ptr: Long)
    }

    object RfcommSocket : BluetoothNativeGateway.RfcommSocketGateway {
        init { Win32NativeGateway.hashCode() }
        external override fun connect(ptr: Long, timeoutMs: Long, callback: Callback)
        external override fun cancelConnect(ptr: Long)
        external override fun startReading(ptr: Long, callback: Callback)
        external override fun stopReading(ptr: Long)
        external override fun write(ptr: Long, data: ByteArray)
        external override fun close(ptr: Long)
        external override fun release(ptr: Long)
    }
}