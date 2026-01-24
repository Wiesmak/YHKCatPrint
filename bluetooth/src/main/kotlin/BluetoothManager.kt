/*
 * Copyright (c) 2026. Umamusume Polska
 */

import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext

class BluetoothManager(
    private val gateway: BluetoothNativeGateway,
) : AutoCloseable {
    private val managerGateway = gateway.bluetoothManager
    private val ptr: Long = managerGateway.init()
    private val cleanable = NativeCleaner.register(
        this,NativeDeallocator(ptr, managerGateway)
    )

    private suspend fun getAdapterByPtr(adapterPtr: Long): BluetoothAdapter = withContext(Dispatchers.IO) {
        val info = AdapterInfo(
            address = managerGateway.getAdapterAddress(adapterPtr),
            name = managerGateway.getAdapterName(adapterPtr)
        )
        BluetoothAdapter(adapterPtr, info, gateway)
    }

    suspend fun listAdapters(): List<BluetoothAdapter> = withContext(Dispatchers.IO) {
        val pointers = managerGateway.listAdapters(ptr)
        pointers.map { getAdapterByPtr(it) }
    }

    fun getAdapter(address: String): Any {
        return managerGateway.getAdapter(ptr, address)
    }

    override fun close() {
        cleanable.clean()
    }
}