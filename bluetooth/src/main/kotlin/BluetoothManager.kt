/*
 * Copyright (c) 2026. Umamusume Polska
 */

import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import java.lang.ref.Cleaner

class BluetoothManager : AutoCloseable {
    private val ptr: Long = nativeInit()
    private val cleanable = cleaner.register(this, NativeDeallocator())

    companion object {
        private val cleaner = Cleaner.create()

        private external fun nativeInit(): Long

        private external fun nativeListAdapters(ptr: Long): List<AdapterInfo>

        private external fun nativeGetAdapter(address: String): BluetoothAdapter

        private external fun nativeRelease(ptr: Long)
    }

    private class NativeDeallocator(private val ptr: Long) : Runnable {
        override fun run() {
            nativeRelease(ptr)
        }
    }

    suspend fun listAdapters(): List<Any> = withContext(Dispatchers.IO) {
        nativeListAdapters(ptr)
    }

    fun getAdapter(address: String): Any {
        return nativeGetAdapter(address)
    }

    override fun close() {
        cleanable.clean()
    }
}