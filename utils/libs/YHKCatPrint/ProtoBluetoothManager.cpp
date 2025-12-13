/*++

Copyright (C) 2025 Umamusume Polska

Module Name:
	ProtoBluetoothManager.cpp

Abstract:
	Implementation of ProtoBluetoothManager methods.

--*/

#include "ProtoBluetoothManager.h"
#include "ProtoAdapter.h"
#include <bluetoothapis.h>
#include <stdexcept>
#include <ranges>
#include <vector>
#include <memory>
#include <iostream>

yhkcatprint::ProtoBluetoothManager::ProtoBluetoothManager()
{
	BLUETOOTH_FIND_RADIO_PARAMS params = { sizeof(BLUETOOTH_FIND_RADIO_PARAMS) };
	HANDLE hRadio = nullptr;
	HBLUETOOTH_RADIO_FIND hFind = BluetoothFindFirstRadio(&params, &hRadio);

	if (hFind == nullptr)
	{
		std::cout << "No Bluetooth adapters found on this device." << std::endl;
		return;
	}

	do
	{
		BLUETOOTH_RADIO_INFO radioInfo = { sizeof(BLUETOOTH_RADIO_INFO) };

		DWORD result = BluetoothGetRadioInfo(hRadio, &radioInfo);

		if (result == ERROR_SUCCESS)
		{
			yhkcatprint::ADAPTER_INFO info;
			info.address = formatBthAddr(radioInfo.address.ullLong);
			info.name = wideStringToString(radioInfo.szName);
			adapters.push_back(std::make_shared<ProtoAdapter>());
		}
		else
		{
			std::cerr << "Failed to retrieve radio info. Error: " << result << std::endl;
		}

		CloseHandle(hRadio);

	} while (BluetoothFindNextRadio(hFind, &hRadio));
	BluetoothFindRadioClose(hFind);
}

std::vector<yhkcatprint::ADAPTER_INFO> yhkcatprint::ProtoBluetoothManager::listAdapters()
{
	constexpr auto getInfo = [](const std::shared_ptr<IAdapter>& adapter)
	{
		return adapter->getInfo();
	};

	std::vector<ADAPTER_INFO> adapterInfos;
	
	for (ADAPTER_INFO info : adapters | std::views::transform(getInfo))
	{
		adapterInfos.push_back(info);
	}

	return adapterInfos;
}

std::shared_ptr<yhkcatprint::IAdapter> yhkcatprint::ProtoBluetoothManager::getAdapter()
{
	return adapters.empty() ? nullptr : adapters.front();
}

std::shared_ptr<yhkcatprint::IAdapter> yhkcatprint::ProtoBluetoothManager::getAdapter(
	const std::string& adapterAddress)
{
	auto result = std::ranges::find_if(adapters, [&](const std::shared_ptr<IAdapter>& adapter)
		{
			return adapter && adapter->getInfo().address == adapterAddress;
		});

	return result != std::ranges::end(adapters) ? *result : nullptr;
}

void yhkcatprint::ProtoBluetoothManager::shutdown()
{
	std::cout << "ProtoBluetoothManager shutting down." << std::endl;
	adapters.clear();
}

std::string yhkcatprint::ProtoBluetoothManager::wideStringToString(const std::wstring& wstr)
{
	std::string str(wstr.begin(), wstr.end());
}

std::string yhkcatprint::ProtoBluetoothManager::formatBthAddr(const BTH_ADDR& addr)
{
	std::ostringstream oss;
	oss << std::hex << std::setfill('0')
		<< std::setw(2) << ((addr >> 40) & 0xFF) << ":"
		<< std::setw(2) << ((addr >> 32) & 0xFF) << ":"
		<< std::setw(2) << ((addr >> 24) & 0xFF) << ":"
		<< std::setw(2) << ((addr >> 16) & 0xFF) << ":"
		<< std::setw(2) << ((addr >> 8) & 0xFF) << ":"
		<< std::setw(2) << (addr & 0xFF);
	return oss.str();
}