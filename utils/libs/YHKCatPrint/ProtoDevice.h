/*++

Copyright (C) 2025 Umamusume Polska

Module Name:
	ProtoRfcommSocket.h

Abstract:
	Prototype implementation of IDevice using ProtoRfcommSocket

--*/

#pragma once
#include "IDevice.h"
#include <cstdint>
#include <string>
#include <memory>

/**
 * @file ProtoRfcommSocket.h
 * @brief Prototype implementation of IDevice using ProtoRfcommSocket.
 *
 * This header defines a concrete class that implements IDevice, using ProtoRfcommSocket.
 * For testing purposes only.
 */

namespace yhkcatprint
{
	/**
	 * @brief Prototype implementation of IDevice using ProtoRfcommSocket.
	 * 
	 * Prototype implementation of IDevice that uses ProtoRfcommSocket class.
	 * Provides standalone device, with RFCOMM socket communication functionality.
	 * 
	 * For testing purposes only, not intended for production use.
	 */
	class ProtoDevice : public IDevice
	{
	public:
		/**
		 * @brief Constructs a ProtoDevice.
		 * @param address Bluetooth MAC address of the device.
		 * @param name Human-readable device name
		 */
		ProtoDevice(const std::string& address, const std::string& name);

		/**
		 * @brief Virtual destructor.
		 */
		virtual ~ProtoDevice() = default;

		DEVICE_INFO getInfo() override;

		std::shared_ptr<IRfcommSocket> createRfcommSocket(uint8_t channel, ConnectOptions options) override;

	private:
		/**
		 * @brief Device bluetooth MAC address
		 */
		std::string deviceAddress;
		/**
		 * @brief Human-readable device name
		 */
		std::string deviceName;
	};
}
