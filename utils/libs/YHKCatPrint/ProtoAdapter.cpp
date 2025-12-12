/*++

Copyright (C) 2025 Umamusume Polska

Module Name:
	ProtoRfcommSocket.cpp

Abstract:
	Implementation of ProtoAdapter methods.

--*/

#include "ProtoAdapter.h"
#include "ProtoDevice.h"
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <iomanip>

yhkcatprint::ADAPTER_INFO yhkcatprint::ProtoAdapter::getInfo()
{
	BLUETOOTH_FIND_RADIO_PARAMS params = { sizeof(BLUETOOTH_FIND_RADIO_PARAMS) };
	HANDLE hRadio = nullptr;
	HBLUETOOTH_RADIO_FIND hFind = BluetoothFindFirstRadio(&params, &hRadio);

	if (hFind == nullptr)
	{
		throw std::runtime_error("Failed to find Bluetooth radio.");
	}

	BLUETOOTH_RADIO_INFO radioInfo = { sizeof(BLUETOOTH_RADIO_INFO) };

	if (BluetoothGetRadioInfo(hRadio, &radioInfo) != ERROR_SUCCESS)
	{
		BluetoothFindRadioClose(hFind);
		CloseHandle(hRadio);
		throw std::runtime_error("Failed to get Bluetooth radio info.");
	}

	BluetoothFindRadioClose(hFind);
	CloseHandle(hRadio);
	ADAPTER_INFO info;
	info.address = formatBthAddr(radioInfo.address.ullLong);
	info.name = wideStringToString(radioInfo.szName);

	return info;
}

std::vector<std::shared_ptr<yhkcatprint::IDevice>> yhkcatprint::ProtoAdapter::getPairedDevices()
{
	std::vector<std::shared_ptr<IDevice>> devices;

	BLUETOOTH_DEVICE_SEARCH_PARAMS searchParams = { sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS) };
	searchParams.fReturnAuthenticated = TRUE;
	searchParams.fReturnRemembered = TRUE;
	searchParams.fReturnUnknown = FALSE;
	searchParams.fReturnConnected = FALSE;
	searchParams.fIssueInquiry = FALSE;
	searchParams.cTimeoutMultiplier = 0;

	BLUETOOTH_DEVICE_INFO deviceInfo = { sizeof(BLUETOOTH_DEVICE_INFO) };
	HBLUETOOTH_DEVICE_FIND hFind = BluetoothFindFirstDevice(&searchParams, &deviceInfo);

	if (hFind == nullptr)
	{
		throw std::runtime_error("Failed to find Bluetooth devices.");
	}

	do
	{
		std::string address = formatBthAddr(deviceInfo.Address.ullLong);
		std::string name = wideStringToString(deviceInfo.szName);
		devices.push_back(std::make_shared<ProtoDevice>(address, name));
	} while (BluetoothFindNextDevice(hFind, &deviceInfo));

	BluetoothFindDeviceClose(hFind);

	return devices;
}

std::string yhkcatprint::ProtoAdapter::wideStringToString(const std::wstring& wstr)
{
	std::string str(wstr.begin(), wstr.end());
	return str;
}

std::string yhkcatprint::ProtoAdapter::formatBthAddr(const BTH_ADDR& addr)
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