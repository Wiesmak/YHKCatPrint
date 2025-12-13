/*++

Copyright (C) 2025 Umamusume Polska

Module Name:
	win32_device.cpp

Abstract:
	Implementation of DeviceWin32 methods.

--*/

module yhkcatprint.win32:device;

import std;
import yhkcatprint;

/**
 * @file win32_device.cpp
 * @brief Implementation of DeviceWin32 methods.
 * 
 * This file provides the Windows-specific implementation of the DeviceWin32
 * class using the Windows Bluetooth API.
 */

namespace yhkcatprint
{
	template<std::derived_from<IRfcommSocket> TSocket>
	yhkcatprint::DeviceWin32<TSocket>::DeviceWin32(const std::string& address, const std::string& name)
		: deviceAddress(address), deviceName(name)
	{
	}

	template<std::derived_from<IRfcommSocket> TSocket>
	DEVICE_INFO DeviceWin32<TSocket>::getInfo() noexcept
	{
		DEVICE_INFO info;
		info.address = deviceAddress;
		info.name = deviceName;
		return info;
	}

	template<std::derived_from<IRfcommSocket> TSocket>
	std::shared_ptr<TSocket> DeviceWin32<TSocket>::createRfcommSocket(std::uint8_t channel, ConnectOptions options)
	{
		if (channel < 1 || channel > 30)
		{
			throw std::invalid_argument("Invalid RFCOMM channel number");
		}

		auto socket = std::make_shared<TSocket>(deviceAddress, channel);

		switch (options)
		{
		case TIMEOUT_DEFAULT:
			socket->connect(std::chrono::seconds(5));
			break;
		case TIMEOUT_LONG:
			socket->connect(std::chrono::seconds(30));
		case TIMEOUT_NONE:
			[[fallthrough]];
		default:
			socket->connect();
			break;
		}

		return socket;
	}
}

