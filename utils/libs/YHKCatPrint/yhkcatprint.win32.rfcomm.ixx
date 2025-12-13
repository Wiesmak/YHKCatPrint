/*++

Copyright (C) 2025 Umamusume Polska

Module Name:
	yhkcatprint.win32.rfcomm.ixx

Abstract:
	Win32 implementation of IRfcommSocket interface.

--*/

export module yhkcatprint.win32:rfcomm;

import std;
import yhkcatprint;

/**
 * @file yhkcatprint.win32.rfcomm.ixx
 * @brief Win32 implementation of IRfcommSocket interface.
 * 
 * This module provides a Windows-specific implementation of the IRfcommSocket
 * interface using the Windows Sockets API.
 */

export namespace yhkcatprint
{
	/**
	 * @brief Win32 implementation of IRfcommSocket interface.
	 * 
	 * This class implements the IRfcommSocket interface using the Windows Sockets API.
	 * It provides RFCOMM socket communication functionality on Windows platforms.
	 */
	class RfcommSocketWin32 : public IRfcommSocket
	{
	public:
		/**
		 * @brief Constructs an RfcommSocketWin32.
		 * 
		 * @param address Bluetooth address of the remote device in format "XX:XX:XX:XX:XX:XX".
		 * @param channel RFCOMM channel number to connect to.
		 * 
		 * @throws std::runtime_error on failure to create the socket.
		 * @throws std::invalid_argument if the address format is invalid.
		 */
		RfcommSocketWin32(const std::string& address, std::uint8_t channel);

		/**
		 * @brief Destructor. Closes the socket if open.
		 */
		virtual ~RfcommSocketWin32();

		// Disable copy semantics
		RfcommSocketWin32(const RfcommSocketWin32&) = delete;
		RfcommSocketWin32& operator=(const RfcommSocketWin32&) = delete;

		// Enable move semantics
		RfcommSocketWin32(RfcommSocketWin32&&) noexcept;
		RfcommSocketWin32& operator=(RfcommSocketWin32&&) noexcept;

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
		void connect() override;

		/**
		 * @brief Establishes a connection to the remote RFCOMM device with a timeout.
		 * 
		 * @param timeout Maximum duration to wait for the connection to be established.
		 *
		 * @pre The socket is not currently connected.
		 * @post On success, the socket is connected and ready for I/O.
		 * 
		 * @throws std::runtime_error on failure to connect or timeout.
		 */
		void connect(std::chrono::nanoseconds timeout) override;

		/**
		 * @brief Sends data over the RFCOMM connection.
		 * 
		 * @param data Pointer to the data to send.
		 * @param size Number of bytes to send.
		 * @return Number of bytes actually sent.
		 * 
		 * @pre The socket is connected.
		 * @throws std::runtime_error on send failure.
		 */
		size_t send(const std::uint8_t* data, size_t size) override;

		/**
		 * @brief Receives data from the RFCOMM connection.
		 * 
		 * @param buffer Pointer to the buffer to store received data.
		 * @param size Maximum number of bytes to receive.
		 * @return Number of bytes actually received.
		 * 
		 * @pre The socket is connected.
		 * @throws std::runtime_error on receive failure.
		 */
		size_t receive(std::uint8_t* buffer, size_t size) override;

		/**
		 * @brief Checks if data is available to read from the RFCOMM connection.
		 * 
		 * @return true if data is available to read, false otherwise.
		 * 
		 * @pre The socket is connected.
		 */
		bool available() override;

		/**
		 * @brief Closes the RFCOMM socket.
		 * 
		 * @post The socket is closed and no longer connected.
		 */
		void close() override;

	private:
		/**
		 * @brief Implementation struct to hide platform-specific details.
		 */
		struct Impl;

		/**
		 * @brief Pointer to the implementation.
		 */
		std::unique_ptr<Impl> m_impl;
	};
}