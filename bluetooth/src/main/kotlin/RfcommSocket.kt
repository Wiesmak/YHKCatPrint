/*
 * Copyright (c) 2026. Umamusume Polska
 */

package pl.umamusume.yhkcatprint.bluetooth

import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.channels.awaitClose
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.callbackFlow
import kotlinx.coroutines.suspendCancellableCoroutine
import kotlinx.coroutines.withContext
import javax.security.auth.callback.Callback
import kotlin.coroutines.resumeWithException

class RfcommSocket(
    private val ptr: Long,
    gateway: BluetoothNativeGateway
) : AutoCloseable {
    private val rfcommSocketGateway = gateway.rfcommSocket
    private val cleanable = NativeCleaner.register(
        this, NativeDeallocator(ptr, rfcommSocketGateway)
    )

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

            rfcommSocketGateway.connect(ptr, timeoutMs, callback)

            continuation.invokeOnCancellation {
                rfcommSocketGateway.cancelConnect(ptr)
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

        rfcommSocketGateway.startReading(ptr, callback)

        awaitClose {
            rfcommSocketGateway.close(ptr)
        }
    }

    suspend fun write(data: ByteArray) = withContext(Dispatchers.IO) {
        rfcommSocketGateway.write(ptr, data)
    }

    override fun close() {
        cleanable.clean()
    }
}