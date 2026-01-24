/*
 * Copyright (c) 2026. Umamusume Polska
 */

import java.lang.ref.Cleaner

object NativeCleaner {
    fun register (obj: Any, deallocator: Runnable): Cleaner.Cleanable {
        return instance.register(obj, deallocator)
    }
    val instance: Cleaner = java.lang.ref.Cleaner.create()
}