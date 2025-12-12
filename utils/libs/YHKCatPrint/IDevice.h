/*++

Copyright (C) 2025 Umamusume Polska

Module Name:
	IDevice.h

Abstract:
	Abstract interface representing a remote Bluetooth device.

--*/

#pragma once
#include <string>
#include "IRfcommSocket.h"

/**
 * @file IDevice.h
 * @brief Defines an abstract interface representing a remote Bluetooth device.
 *
 * This header provides an interface for interacting with Bluetooth devices,
 * including retrieving device information and creating RFCOMM sockets.
 */

namespace yhkcatprint
{
	/**
	 * @brief Structure containing Bluetooth device information.
	 */
	typedef struct _DEVICE_INFO
	{
		/**
		 * @brief Bluetooth address of the device in format "XX:XX:XX:XX:XX:XX".
		 */
		std::string address;
		/**
		 * @brief Human-readable name of the device.
		 */
		std::string name;
	} DEVICE_INFO;

	/**
	 * @brief Connection options for RFCOMM sockets.
	 */
	enum ConnectOptions
	{
		/**
		 * @brief No timeout; block indefinitely.
		 */
		TIMEOUT_NONE = 0,
		/**
		 * @brief Default timeout duration.
		 */
		TIMEOUT_DEFAULT = 1,
		/**
		 * @brief Long timeout duration.
		 */
		TIMEOUT_LONG = 2
	};

	/**
	 * @brief Abstract interface representing a remote Bluetooth device.
	 * 
	 * Implementations of this interface provide methods to retrieve device
	 * information and create RFCOMM sockets for communication.
	 */
	class IDevice
	{
	public:
		/**
		 * @brief Virtual destructor.
		 */
		virtual ~IDevice() = default;

		/**
		 * @brief Retrieves information about the Bluetooth device.
		 * 
		 * @return DEVICE_INFO structure containing the device's address and name.
		 */
		virtual DEVICE_INFO getInfo() = 0;

		/**
		 * @brief Creates an RFCOMM socket connected to the specified channel.
		 * 
		 * @param channel RFCOMM channel number to connect to.
		 * @param options Connection options (e.g., timeout settings).
		 * 
		 * @return Shared pointer to the created IRfcommSocket.
		 * 
		 * @throws std::runtime_error on failure to create the socket.
		 */
		virtual std::shared_ptr<IRfcommSocket> createRfcommSocket(uint8_t channel, ConnectOptions options) = 0;
	};
}

