/*++

Copyright (C) 2025 Umamusume Polska

Module Name:
	ProtoAdapter.h

Abstract:
	Prototype implementation of IAdapter using ProtoDevice

--*/

#pragma once
#include "IAdapter.h"
#include <string>
#include <vector>
#include <memory>
#include <WinSock2.h>
#include <ws2bth.h>
#include <BluetoothAPIs.h>

// Link with Bthprops.lib
#pragma comment(lib, "Bthprops.lib")

/**
 * @file ProtoRfcommSocket.h
 * @brief Prototype implementation of IAdapter using ProtoDevice.
 *
 * This header defines a concrete class that implements IAdapter, using ProtoDevice.
 * For testing purposes only.
 */

namespace yhkcatprint {
	/**
	 * @brief Prototype implementation of IAdapter using ProtoDevice.
	 * 
	 * Prototype implementation of IAdapter that uses ProtoDevice class.
	 * Provides standalone adapter, with paired device enumeration functionality.
	 * 
	 * For testing purposes only, not intended for production use.
	 */
	class ProtoAdapter : public IAdapter
	{
	public:
		/**
		 * @brief Constructs a ProtoAdapter.
		 */
		ProtoAdapter() = default;

		/**
		 * @brief Virtual destructor.
		 */
		virtual ~ProtoAdapter() = default;

		ADAPTER_INFO getInfo() override;

		std::vector<std::shared_ptr<IDevice>> getPairedDevices() override;

	private:
		/**
		 * @brief Converts a wide string (std::wstring) to a standard string (std::string).
		 * @param wstr The wide string to convert.
		 * @return The converted standard string.
		 */
		static std::string wideStringToString(const std::wstring& wstr);

		/**
		 * @brief Formats a BTH_ADDR into a human-readable MAC address string.
		 * @param addr The BTH_ADDR to format.
		 * @return The formatted MAC address string in the format "XX:XX:XX:XX:XX:XX".
		 */
		static std::string formatBthAddr(const BTH_ADDR& addr);
	};
}

