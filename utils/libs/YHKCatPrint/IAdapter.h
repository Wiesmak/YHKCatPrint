/*++

Copyright (C) 2025 Umamusume Polska

Module Name:
	IAdapter.h

Abstract:
	Abstract interface representing device Bluetooth adapter.

--*/

#pragma once

#include "IDevice.h"
#include <vector>

namespace yhkcatprint
{
	/**
	 * @brief Structure containing Bluetooth adapter information.
	 */
	typedef struct _ADAPTER_INFO
	{
		/**
		 * @brief Adapter Bluetooth address in format "XX:XX:XX:XX:XX:XX".
		 */
		std::string address;
		/**
		 * @brief Human-readable name of the adapter.
		 */
		std::string name;
	} ADAPTER_INFO;

	/**
	 * @brief Abstract interface representing device Bluetooth adapter.
	 */
	class IAdapter
	{
	public:
		/**
		 * @brief Virtual destructor.
		 */
		virtual ~IAdapter() = default;

		/**
		 * @brief Retrieves information about the Bluetooth adapter.
		 * 
		 * @return ADAPTER_INFO structure containing the adapter's address and name.
		 */
		virtual ADAPTER_INFO getInfo() = 0;

		/**
		 * @brief Retrieves a list of paired Bluetooth devices.
		 * 
		 * @return Vector of shared pointers to IDevice representing paired devices.
		 */
		virtual std::vector<std::shared_ptr<IDevice>> getPairedDevices() = 0;
	};
}

