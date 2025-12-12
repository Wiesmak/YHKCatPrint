/*++

Copyright (C) 2025 Umamusume Polska

Module Name:
	ProtoRfcommSocket.cpp

Abstract:
	Implementation of ProtoRfcommSocket methods.

--*/

#include "ProtoRfcommSocket.h"
#include <stdexcept>
#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>

yhkcatprint::ProtoRfcommSocket::ProtoRfcommSocket(const std::string& address, uint8_t channel)
	: m_socket(INVALID_SOCKET), m_connected(false)
{
	ensureWinsockInit();
	std::memset(&m_addr, 0, sizeof(m_addr));
	m_addr.addressFamily = AF_BTH;
	m_addr.btAddr = strToBthAddr(address);
	m_addr.serviceClassId = RFCOMM_PROTOCOL_UUID;
	m_addr.port = static_cast<ULONG>(channel);

	m_socket = ::socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
	if (m_socket == INVALID_SOCKET) {
		throw std::runtime_error("Failed to create socket");
	}
}

yhkcatprint::ProtoRfcommSocket::~ProtoRfcommSocket()
{
	close();
}

void yhkcatprint::ProtoRfcommSocket::connect()
{
	if (m_connected) {
		throw std::runtime_error("Socket already connected");
	}
	if (::connect(m_socket, reinterpret_cast<SOCKADDR*>(&m_addr), sizeof(m_addr)) == SOCKET_ERROR) {
		throw std::runtime_error("Failed to connect to the device");
	}
	m_connected = true;
}

void yhkcatprint::ProtoRfcommSocket::connect(std::chrono::nanoseconds timeout)
{
	// For simplicity, ignoring timeout implementation in this example
	connect();
}

size_t yhkcatprint::ProtoRfcommSocket::send(const uint8_t* data, size_t size)
{
	if (!m_connected) {
		throw std::runtime_error("Socket not connected");
	}
	int bytesSent = ::send(m_socket, reinterpret_cast<const char*>(data), static_cast<int>(size), 0);
	if (bytesSent == SOCKET_ERROR) {
		throw std::runtime_error("Failed to send data");
	}
	return static_cast<size_t>(bytesSent);
}

size_t yhkcatprint::ProtoRfcommSocket::receive(uint8_t* buffer, size_t size)
{
	if (!m_connected) {
		throw std::runtime_error("Socket not connected");
	}
	int bytesReceived = ::recv(m_socket, reinterpret_cast<char*>(buffer), static_cast<int>(size), 0);
	if (bytesReceived == SOCKET_ERROR) {
		throw std::runtime_error("Failed to receive data");
	}
	if (bytesReceived == 0) {
		// Connection has been closed
		m_connected = false;
	}
	return static_cast<size_t>(bytesReceived);
}

bool yhkcatprint::ProtoRfcommSocket::available()
{
	if (!m_connected) {
		return false;
	}
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(m_socket, &readfds);
	timeval timeout = { 0, 0 }; // Non-blocking check
	int result = ::select(0, &readfds, nullptr, nullptr, &timeout);
	return (result > 0);
}

void yhkcatprint::ProtoRfcommSocket::close()
{
	if (m_socket != INVALID_SOCKET) {
		::closesocket(m_socket);
		m_socket = INVALID_SOCKET;
		m_connected = false;
	}
}

BTH_ADDR yhkcatprint::ProtoRfcommSocket::strToBthAddr(const std::string& address)
{
	BTH_ADDR btAddr = 0;
	std::istringstream iss(address);
	std::string byteStr;
	for (int i = 0; i < 6; ++i) {
		if (!std::getline(iss, byteStr, ':')) {
			throw std::invalid_argument("Invalid Bluetooth address format");
		}
		uint8_t byte = static_cast<uint8_t>(std::stoul(byteStr, nullptr, 16));
		btAddr |= (static_cast<BTH_ADDR>(byte) << (8 * (5 - i)));
	}
	return btAddr;
}

void yhkcatprint::ProtoRfcommSocket::ensureWinsockInit()
{
	static bool initialized = false;
	if (!initialized) {
		WSADATA wsaData;
		int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (result != 0) {
			throw std::runtime_error("Failed to initialize Winsock");
		}
		initialized = true;
	}
}