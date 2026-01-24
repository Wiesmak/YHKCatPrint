/*
 * Copyright (c) 2026. Umamusume Polska
 */

package pl.umamusume.yhkcatprint.bluetooth

import java.lang.ref.Cleaner

object NativeCleaner {
    fun register (obj: Any, deallocator: Runnable): Cleaner.Cleanable {
        return instance.register(obj, deallocator)
    }
    val instance: Cleaner = java.lang.ref.Cleaner.create()
}