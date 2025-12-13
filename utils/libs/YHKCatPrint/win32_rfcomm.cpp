/*++

Copyright (C) 2025 Umamusume Polska

Module Name:
	win32_rfcomm.cpp

Abstract:
	Implementation of RfcommSocketWin32 methods.

--*/

module;

#include <WinSock2.h>
#include <WS2bth.h>

// Link with ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

module yhkcatprint.win32:rfcomm;

import std;

/**
 * @file win32_rfcomm.cpp
 * @brief Implementation of RfcommSocketWin32 methods.
 * 
 * This file provides the Windows-specific implementation of the RfcommSocketWin32
 * class using the Windows Sockets API.
 */

namespace yhkcatprint
{
	struct RfcommSocketWin32::Impl
	{
		SOCKET socket;
		SOCKADDR_BTH addr;
		bool connected;

		BTH_ADDR strToBthAddr(const std::string& address)
		{
			BTH_ADDR btAddr = 0;
			std::istringstream iss(address);
			std::string byteStr;
			for (int i = 0; i < 6; ++i) {
				if (!std::getline(iss, byteStr, ':')) {
					throw std::invalid_argument("Invalid Bluetooth address format");
				}
				std::uint8_t byte = static_cast<std::uint8_t>(std::stoul(byteStr, nullptr, 16));
				btAddr |= (static_cast<BTH_ADDR>(byte) << (8 * (5 - i)));
			}
			return btAddr;
		}

		void ensureWinsockInit()
		{
			static bool initialized = false;
			if (!initialized) {
				WSADATA wsaData;
				int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
				if (result != 0) {
					throw std::runtime_error("WSAStartup failed");
				}
				initialized = true;
			}
		}

		Impl() : socket(INVALID_SOCKET), connected(false)
		{
			std::memset(&addr, 0, sizeof(addr));
		}
	};

	RfcommSocketWin32::RfcommSocketWin32(const std::string& address, std::uint8_t channel)
		: m_impl(std::make_unique<Impl>())
	{
		m_impl->ensureWinsockInit();
		m_impl->addr.addressFamily = AF_BTH;
		m_impl->addr.btAddr = m_impl->strToBthAddr(address);
		m_impl->addr.serviceClassId = RFCOMM_PROTOCOL_UUID;
		m_impl->addr.port = static_cast<ULONG>(channel);

		m_impl->socket = ::socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
		if (m_impl->socket == INVALID_SOCKET) {
			throw std::runtime_error("Failed to create socket");
		}
	}

	RfcommSocketWin32::~RfcommSocketWin32()
	{
		close();
	}

	RfcommSocketWin32::RfcommSocketWin32(RfcommSocketWin32&& other) noexcept = default;
	RfcommSocketWin32& RfcommSocketWin32::operator=(RfcommSocketWin32&& other) noexcept = default;

	void RfcommSocketWin32::connect()
	{
		if (m_impl->connected) {
			throw std::runtime_error("Socket already connected");
		}
		if (::connect(m_impl->socket, reinterpret_cast<SOCKADDR*>(&m_impl->addr), sizeof(m_impl->addr)) == SOCKET_ERROR) {
			throw std::runtime_error("Failed to connect to the device");
		}
		m_impl->connected = true;
	}

	void RfcommSocketWin32::connect(std::chrono::nanoseconds timeout)
	{
		// For simplicity, ignoring timeout implementation in this example
		connect();
	}

	size_t RfcommSocketWin32::send(const std::uint8_t* data, size_t size)
	{
		if (!m_impl->connected) {
			throw std::runtime_error("Socket not connected");
		}
		int bytesSent = ::send(m_impl->socket, reinterpret_cast<const char*>(data), static_cast<int>(size), 0);
		if (bytesSent == SOCKET_ERROR) {
			throw std::runtime_error("Failed to send data");
		}
		return static_cast<size_t>(bytesSent);
	}

	size_t RfcommSocketWin32::receive(std::uint8_t* buffer, size_t size)
	{
		if (!m_impl->connected) {
			throw std::runtime_error("Socket not connected");
		}
		int bytesReceived = ::recv(m_impl->socket, reinterpret_cast<char*>(buffer), static_cast<int>(size), 0);
		if (bytesReceived == SOCKET_ERROR) {
			throw std::runtime_error("Failed to receive data");
		}
		if (bytesReceived == 0) {
			// Connection has been closed
			m_impl->connected = false;
		}
		return static_cast<size_t>(bytesReceived);
	}

	size_t RfcommSocketWin32::available()
	{
		if (!m_impl->connected) {
			throw std::runtime_error("Socket not connected");
		}
		u_long bytesAvailable = 0;
		if (ioctlsocket(m_impl->socket, FIONREAD, &bytesAvailable) == SOCKET_ERROR) {
			throw std::runtime_error("Failed to check available data");
		}
		return bytesAvailable > 0;
	}

	void RfcommSocketWin32::close()
	{
		if (m_impl->socket != INVALID_SOCKET) {
			::closesocket(m_impl->socket);
			m_impl->socket = INVALID_SOCKET;
			m_impl->connected = false;
		}
	}
}