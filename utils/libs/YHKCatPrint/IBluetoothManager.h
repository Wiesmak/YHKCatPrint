/*++

Copyright (C) 2025 Umamusume Polska

Module Name:
	IAdapter.h

Abstract:
	Abstract interface representing entry point for Bluetooth management.

--*/

#pragma once
#include "IAdapter.h"
#include <vector>
#include <memory>

/**
 * @file IBluetoothManager.h
 * @brief Abstract interface for Bluetooth management.
 * 
 * This header defines the IBluetoothManager interface,
 * which serves as the entry point for managing Bluetooth adapters.
 */

namespace yhkcatprint
{
	/**
	 * @brief Abstract interface representing entry point for Bluetooth management.
	 * 
	 * Manages Bluetooth adapters and global Bluetooth operations.
	 */
	class IBluetoothManager
	{
	public:
		/**
		 * @brief Virtual destructor.
		 */
		virtual ~IBluetoothManager() = default;

		/**
		 * @brief Lists available Bluetooth adapters.
		 * 
		 * @return Vector of ADAPTER_INFO structures for each available adapter.
		 */
		virtual std::vector<ADAPTER_INFO> listAdapters() = 0;

		/**
		 * @brief Retrieves the default Bluetooth adapter.
		 * 
		 * @return Shared pointer to IAdapter representing the default adapter.
		 */
		virtual std::shared_ptr<IAdapter> getAdapter() = 0;

		/**
		 * @brief Retrieves a Bluetooth adapter by its address.
		 * 
		 * @param adapterAddress The Bluetooth address of the adapter in format "XX:XX:XX:XX:XX:XX".
		 * @return Shared pointer to IAdapter representing the requested adapter.
		 */
		virtual std::shared_ptr<IAdapter> getAdapter(const std::string& adapterAddress) = 0;

		/**
		 * @brief Shuts down the Bluetooth manager and releases resources.
		 */
		virtual void shutdown() = 0;
	};
}
