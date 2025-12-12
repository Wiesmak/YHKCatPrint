/*++

Copyright (C) 2025 Umamusume Polska

Module Name:
	IRfcommSocket.h

Abstract:
	Abstract interface for RFCOMM socket communication.

--*/

#pragma once
#include <chrono>

/**
 * @file IRfcommSocket.h
 * @brief Defines an abstract interface for RFCOMM socket communication.
 *
 * This header provides a minimal, implementation-agnostic interface for
 * connecting, sending, receiving, checking readability, and closing an
 * RFCOMM-based connection.
 */

namespace yhkcatprint
{
	/**
	 * @brief Abstract interface for RFCOMM socket communication.
	 * 
	 * Implementations of this interface provide a byte-oriented, stream-like API
	 * over an RFCOMM connection.
	 *
	 * @note Unless specified otherwise by an implementation, methods may block.
	 * @note Implementations should document their thread-safety guarantees.
	 */
	class IRfcommSocket
	{
	public:
		/**
		 * @brief Virtual destructor.
		 */
		virtual ~IRfcommSocket() = default;

		/**
		 * @brief Establishes a connection to the remote RFCOMM device.
		 * 
		* This overload uses an implementation-defined default timeout and/or
         * blocking behavior.
         *
         * @pre The socket is not currently connected.
         * @post On success, the socket is connected and ready for I/O.
		 * 
		 * @throws std::runtime_error on failure to connect.
		 */
		virtual void connect() = 0;

		/**
		 * @brief Establishes a connection to the remote RFCOMM device with a timeout.
		 * 
		 * @param timeout Maximum duration to wait for the connection to be established.
		 *
		 * @pre The socket is not currently connected.
		 * @post On success, the socket is connected and ready for I/O.
		 * 
		 * @throws std::runtime_error on failure to connect or if the timeout is reached.
		 */
		virtual void connect(std::chrono::nanoseconds timeout) = 0;

		/**
		 * @brief Sends data over the RFCOMM connection.
		 * 
		 * @param data Pointer to the data buffer to send.
		 * @param size Number of bytes to send from the buffer.
		 * @return Number of bytes actually sent.
		 * 
		 * @pre The socket is connected.
		 * 
		 * @throws std::runtime_error on failure to send data.
		 */
		virtual size_t send(const uint8_t* data, size_t size) = 0;

		/**
		 * @brief Receives data from the RFCOMM connection.
		 * 
		 * @param buffer Pointer to the buffer to store received data.
		 * @param size Maximum number of bytes to receive.
		 * @return Number of bytes actually received.
		 * 
		 * @pre The socket is connected.
		 * 
		 * @throws std::runtime_error on failure to receive data.
		 */
		virtual size_t receive(uint8_t* buffer, size_t size) = 0;

		/**
		 * @brief Checks if data is available to read from the RFCOMM connection.
		 * 
		 * @return true if data is available to read, false otherwise.
		 * 
		 * @pre The socket is connected.
		 */
		virtual bool available() = 0;

		/**
		 * @brief Closes the RFCOMM connection.
		 * 
		 * @post The socket is no longer connected.
		 */
		virtual void close() = 0;
	};
}
