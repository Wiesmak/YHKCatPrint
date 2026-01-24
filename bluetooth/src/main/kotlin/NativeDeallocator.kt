import kotlinx.coroutines.Runnable

/*
 * Copyright (c) 2026. Umamusume Polska
 */

class NativeDeallocator(
    private val ptr: Long,
    private val releaseGateway: BluetoothNativeGateway.NativeCleaner
) : Runnable {
    override fun run() {
        releaseGateway.release(ptr)
    }
}