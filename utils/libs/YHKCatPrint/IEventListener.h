/*++

Copyright (C) 2025 Umamusume Polska

Module Name:
	IEventListener.h

Abstract:
	Abstract interface for receiving asynchronous events.

--*/

#pragma once
#include "IDevice.h"
#include "IRfcommSocket.h"
#include <string>
#include <cstdint>

/**
 * @file IEventListener.h
 * @brief Abstract interface for receiving asynchronous events.
 * 
 * This header defines the IEventListener interface, which provides
 * asynchronous event callbacks for device connections, disconnections,
 * data reception, socket closures, and error handling.
 */

namespace yhkcatprint
{
	/**
	 * @brief Structure representing error information.
	 */
	typedef struct _ERROR_INFO
	{
		/**
		 * @brief Error code.
		 */
		int code;
		/**
		 * @brief Human-readable error message.
		 */
		std::string message;
	} ERROR_INFO;


	/**
	 * @brief Abstract interface for receiving asynchronous events.
	 * 
	 * This interface defines callback methods for various events such as
	 * device connections, disconnections, data reception, socket closures,
	 * and error handling. Implementers of this interface can handle these
	 * events as needed.
	 */
	class IEventListener
	{
	public:
		/**
		 * @brief Virtual destructor.
		 */
		virtual ~IEventListener() = default;

		/**
		 * @brief Called when a device is connected.
		 * 
		 * @param DEVICE_INFO Information about the connected device.
		 */
		virtual void onDeviceConnected(const DEVICE_INFO&) = 0;

		/**
		 * @brief Called when a device is disconnected.
		 * 
		 * @param DEVICE_INFO Information about the disconnected device.
		 */
		virtual void onDeviceDisconnected(const DEVICE_INFO&) = 0;

		/**
		 * @brief Called when data is received on the RFCOMM socket.
		 * 
		 * @param IRfcommSocket Reference to the RFCOMM socket that received data.
		 * @param data Pointer to the received data buffer.
		 * @param size Size of the received data in bytes.
		 */
		virtual void onSocketDateReceived(
			const IRfcommSocket&,
			const uint8_t* data,
			size_t size) = 0;

		/**
		 * @brief Called when the RFCOMM socket is closed.
		 * @param IRfcommSocket Reference to the RFCOMM socket that was closed.
		 */
		virtual void onSocketClosed(const IRfcommSocket&) = 0;

		/**
		 * @brief Called when an error occurs.
		 * 
		 * @param ERROR_INFO Information about the error.
		 */
		virtual void onError(ERROR_INFO) = 0;
	};
}