/*
 * Copyright (c) 2026. Umamusume Polska
 */

import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.channels.awaitClose
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.callbackFlow
import kotlinx.coroutines.suspendCancellableCoroutine
import kotlinx.coroutines.withContext
import javax.security.auth.callback.Callback
import kotlin.coroutines.resumeWithException

class RfcommSocket(
    private val ptr: Long
) : AutoCloseable {
    private val cleanable = NativeCleaner.register(this, NativeDeallocator(ptr))

    companion object {
        private external fun nativeConnect(ptr: Long, timeoutMs: Long, callback: Callback)

        private external fun nativeCancelConnect(ptr: Long)

        private external fun nativeStartReading(ptr: Long, callback: Callback)

        private external fun nativeStopReading(ptr: Long)

        private external fun nativeWrite(ptr: Long, data: ByteArray)

        private external fun nativeClose(ptr: Long)
    }

    private class NativeDeallocator(private val ptr: Long) : Runnable {
        override fun run() {
            nativeClose(ptr)
        }
    }

    private interface ConnectionCallback : Callback {
        fun onSuccess()
        fun onError(error: Throwable)
    }

    suspend fun connect(timeoutMs: Long = 5000L) {
        suspendCancellableCoroutine<Unit> { continuation ->
            val callback = object : ConnectionCallback {
                override fun onSuccess() {
                    if (continuation.isActive) continuation.resume(Unit) { _, _, _ -> }
                }

                override fun onError(error: Throwable) {
                    if (continuation.isActive) continuation.resumeWithException(error)
                }
            }

            nativeConnect(ptr, timeoutMs, callback)

            continuation.invokeOnCancellation {
                nativeCancelConnect(ptr)
            }
        }
    }

    private interface ReadCallback : Callback {
        fun onDataReceived(data: ByteArray)
        fun onDisconnected()
        fun onError(error: Throwable)
    }

    val incomingData: Flow<ByteArray> = callbackFlow {
        val callback = object : ReadCallback {
            override fun onDataReceived(data: ByteArray) {
                trySend(data).isSuccess
            }

            override fun onDisconnected() {
                close()
            }

            override fun onError(error: Throwable) {
                close(error)
            }
        }

        nativeStartReading(ptr, callback)

        awaitClose {
            nativeStopReading(ptr)
        }
    }

    suspend fun write(data: ByteArray) = withContext(Dispatchers.IO) {
        nativeWrite(ptr, data)
    }

    override fun close() {
        cleanable.clean()
    }
}