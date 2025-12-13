/*++

Copyright (C) 2025 Umamusume Polska

Module Name:
	ProtoBluetoothManager.h

Abstract:
	Prototype implementation of IBluetoothManager using ProtoAdapter.

--*/

#pragma once
#include "IBluetoothManager.h"
#include "IAdapter.h"
#include <vector>
#include <memory>
#include <WinSock2.h>
#include <ws2bth.h>
#include <BluetoothAPIs.h>

// Link with Bthprops.lib
#pragma comment(lib, "Bthprops.lib")

namespace yhkcatprint
{
	/**
	 * @brief Prototype implementation of IBluetoothManager using ProtoAdapter.
	 *
	 * Manages ProtoAdapter instances for Bluetooth management.
	 *
	 * For testing purposes only, not intended for production use.
	 */
	class ProtoBluetoothManager : public IBluetoothManager
	{
	public:
		/**
		 * @brief Constructs a ProtoBluetoothManager.
		 */
		ProtoBluetoothManager();

		/**
		 * @brief Virtual destructor.
		 */
		virtual ~ProtoBluetoothManager() = default;

		std::vector<ADAPTER_INFO> listAdapters() override;

		std::shared_ptr<IAdapter> getAdapter() override;

		std::shared_ptr<IAdapter> getAdapter(const std::string& adapterAddress) override;

		void shutdown() override;

	private:
		/**
		 * @brief List of managed adapters.
		 */
		std::vector<std::shared_ptr<IAdapter>> adapters;

		/**
		 * @brief Converts a wide string (std::wstring) to a standard string (std::string).
		 * @param wstr The wide string to convert.
		 * @return The converted standard string.
		 */
		static std::string wideStringToString(const std::wstring& wstr);

		/**
		 * @brief Formats a BTH_ADDR into a human-readable string.
		 * 
		 * @param addr The BTH_ADDR to format.
		 * @return Formatted Bluetooth address string in "XX:XX:XX:XX:XX:XX" format.
		 */
		static std::string formatBthAddr(const BTH_ADDR& addr);
	};
}
