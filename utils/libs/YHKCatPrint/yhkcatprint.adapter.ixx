/*++

Copyright (C) 2025 Umamusume Polska

Module Name:
	yhkcatprint.adapter.ixx

Abstract:
	Abstract interface representing device Bluetooth adapter.

--*/

export module yhkcatprint:adapter;

import std;
import :device;

/**
 * @file yhkcatprint.adapter.ixx
 * @brief Abstract interface representing device Bluetooth adapter.
 * 
 * This module defines the IAdapter interface,
 * which represents a Bluetooth adapter on the device.
 */

export namespace yhkcatprint
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