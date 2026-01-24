/*
 * Copyright (c) 2026. Umamusume Polska
 */

import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import java.lang.ref.Cleaner

class BluetoothManager : AutoCloseable {
    private val ptr: Long = nativeInit()
    private val cleanable = NativeCleaner.register(this, NativeDeallocator(ptr))

    companion object {
        private external fun nativeInit(): Long

        private external fun nativeListAdapters(ptr: Long): LongArray

        private external fun nativeGetAdapterAddress(ptr: Long): String

        private external fun nativeGetAdapterName(ptr: Long): String

        private external fun nativeGetAdapter(address: String): BluetoothAdapter

        private external fun nativeRelease(ptr: Long)
    }

    private class NativeDeallocator(private val ptr: Long) : Runnable {
        override fun run() {
            nativeRelease(ptr)
        }
    }

    private suspend fun getAdapterByPtr(adapterPtr: Long): BluetoothAdapter = withContext(Dispatchers.IO) {
        val info = AdapterInfo(
            address = nativeGetAdapterAddress(adapterPtr),
            name = nativeGetAdapterName(adapterPtr)
        )
        BluetoothAdapter(adapterPtr, info)
    }

    suspend fun listAdapters(): List<BluetoothAdapter> = withContext(Dispatchers.IO) {
        val pointers = nativeListAdapters(ptr)
        pointers.map { getAdapterByPtr(it) }
    }

    fun getAdapter(address: String): Any {
        return nativeGetAdapter(address)
    }

    override fun close() {
        cleanable.clean()
    }
}