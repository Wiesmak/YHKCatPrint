/*++

Copyright (C) 2025 Umamusume Polska

Module Name:
	ProtoDevice.cpp

Abstract:
	Implementation of ProtoDevice methods.

--*/

#include "ProtoDevice.h"
#include "ProtoRfcommSocket.h"
#include <chrono>
#include <stdexcept>

yhkcatprint::ProtoDevice::ProtoDevice(const std::string& address, const std::string& name)
	: deviceAddress(address), deviceName(name)
{
}

yhkcatprint::DeviceInfo yhkcatprint::ProtoDevice::getInfo()
{
	return { deviceAddress, deviceName };
}

std::shared_ptr<yhkcatprint::IRfcommSocket> yhkcatprint::ProtoDevice::createRfcommSocket(
	uint8_t channel, yhkcatprint::ConnectOptions options)
{
	if (channel < 1 || channel > 30)
	{
		throw std::invalid_argument("Invalid RFCOMM channel number");
	}

	auto socket = std::make_shared<ProtoRfcommSocket>(deviceAddress, channel);

	switch (options)
	{
	case TIMEOUT_DEFAULT:
		socket->connect(std::chrono::seconds(5));
		break;

	case TIMEOUT_LONG:
		socket->connect(std::chrono::seconds(30));

	case TIMEOUT_NONE:
	default:
		socket->connect();
		break;
	}

	return socket;
}