/*++

Copyright (C) 2025 Umamusume Polska

Module Name:
	ProtoRfcommSocket.h

Abstract:
	Prototype implementation of IRfcommSocket using Windows Sockets API.

--*/

#pragma once
#include "IRfcommSocket.h"
#include <cstdint>
#include <string>
#include <winsock2.h>
#include <ws2bth.h>

// Link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

/**
 * @file ProtoRfcommSocket.h
 * @brief Prototype implementation of IRfcommSocket using Windows Sockets API.
 * 
 * This header defines a concrete, standalone class that implements the IRfcommSocket.
 * For testing purposes only.
 */

namespace yhkcatprint
{
	/**
	 * @brief Prototype implementation of IRfcommSocket using Windows Sockets API.
	 * 
	 * Prototype implementation of IRfcommSocket that uses the Windows Sockets API
	 * Provides standalone RFCOMM socket communication functionality.
	 * 
	 * For testing purposes only, not intended for production use.
	 */
	class ProtoRfcommSocket : public IRfcommSocket
	{
	public:
		/**
		 * @brief Constructs a ProtoRfcommSocket.
		 * 
		 * @param address Bluetooth address of the remote device in format "XX:XX:XX:XX:XX:XX".
		 * @param channel RFCOMM channel number to connect to.
		 * 
		 * @throws std::runtime_error on failure to create the socket.
		 * @throws std::invalid_argument if the address format is invalid.
		 */
		ProtoRfcommSocket(const std::string& address, uint8_t channel);

		/**
		 * @brief Destructor. Closes the socket if open.
		 */
		virtual ~ProtoRfcommSocket();

		// Disable copy semantics
		ProtoRfcommSocket(const ProtoRfcommSocket&) = delete;
		ProtoRfcommSocket& operator=(const ProtoRfcommSocket&) = delete;

		void connect() override;
		void connect(std::chrono::nanoseconds timeout) override;
		size_t send(const uint8_t* data, size_t size) override;
		size_t receive(uint8_t* buffer, size_t size) override;
		bool available() override;
		void close() override;

	private:
		SOCKET m_socket;
		SOCKADDR_BTH m_addr;
		bool m_connected;

		/**
		 * @brief Converts a string Bluetooth address to BTH_ADDR.
		 * 
		 * @param address Bluetooth address in format "XX:XX:XX:XX:XX:XX".
		 * @return BTH_ADDR representation of the address.
		 * 
		 * @throws std::invalid_argument if the address format is invalid.
		 */
		static BTH_ADDR strToBthAddr(const std::string& address);

		/**
		 * @brief Ensures that Winsock is initialized.
		 * 
		 * @throws std::runtime_error on failure to initialize Winsock.
		 */
		static void ensureWinsockInit();
	};
}