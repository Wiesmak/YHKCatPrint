/*++

Copyright (C) 2025 Umamusume Polska

Module Name:
	yhkcatprint.win32.device.ixx

Abstract:
	Win32 implementation of IDevice interface.

--*/

export module yhkcatprint.win32:device;

import std;
import yhkcatprint;

/**
 * @file yhkcatprint.win32.device.ixx
 * @brief Win32 implementation of IDevice interface.
 * 
 * This module provides a Windows-specific implementation of the IDevice
 * interface using the Windows Bluetooth API.
 */

export namespace yhkcatprint
{
	/**
	 * @brief Win32 implementation of IDevice interface.
	 * 
	 * This class implements the IDevice interface using the Windows Bluetooth API.
	 * It provides functionality to interact with Bluetooth devices on Windows platforms.
	 * 
	 * @tparam TSocket Type of RFCOMM socket to use, must derive from IRfcommSocket.
	 */
	template<std::derived_from<IRfcommSocket> TSocket>
	class DeviceWin32 : public IDevice
	{
		public:
		/**
		 * @brief Constructs a DeviceWin32.
		 * 
		 * @param address Bluetooth MAC address of the device.
		 * @param name Human-readable device name.
		 */
		DeviceWin32(const std::string& address, const std::string& name);

		/**
		 * @brief Virtual destructor.
		 */
		virtual ~DeviceWin32() = default;

		/**
		 * @brief Retrieves information about the Bluetooth device.
		 * @return DEVICE_INFO structure containing device's address and name.
		 */
		DEVICE_INFO getInfo() noexcept override;

		/**
		 * @brief Creates an RFCOMM socket to the device on the specified channel.
		 * 
		 * @param channel RFCOMM channel number to connect to.
		 * @param options Connection options (e.g., timeout settings).
		 * @return Shared pointer to the created IRfcommSocket.
		 * 
		 * @throws std::runtime_error on failure to create the socket.
		 */
		std::shared_ptr<TSocket> createRfcommSocket(std::uint8_t channel, ConnectOptions options) override;

	private:
		/**
		 * @brief Device bluetooth MAC address
		 */
		std::string deviceAddress;
		/**
		 * @brief Human-readable name of the device.
		 */
		std::string deviceName;
	};
}