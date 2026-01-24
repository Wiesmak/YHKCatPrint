/*
 * Copyright (c) 2026. Umamusume Polska
 */

package pl.umamusume.yhkcatprint.bluetooth

import kotlinx.coroutines.Runnable

class NativeDeallocator(
    private val ptr: Long,
    private val releaseGateway: BluetoothNativeGateway.NativeCleaner
) : Runnable {
    override fun run() {
        releaseGateway.release(ptr)
    }
}