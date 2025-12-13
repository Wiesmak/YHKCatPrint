/*++

Copyright (C) 2025 Umamusume Polska

Module Name:
	win32_adapter.cpp

Abstract:
	Implementation of AdapterWin32 methods.

--*/

module;

#include <WinSock2.h>
#include <ws2bth.h>
#include <BluetoothAPIs.h>

// Link with Bthprops.lib
#pragma comment(lib, "Bthprops.lib")

module yhkcatprint.win32:adapter;

import std;

/**
 * @file win32_adapter.cpp
 * @brief Implementation of AdapterWin32 methods.
 * 
 * This file provides the Windows-specific implementation of the AdapterWin32
 * class using the Windows Bluetooth API.
 */

namespace yhkcatprint
{
	template <std::derived_from<IDevice> TDevice>
	struct AdapterWin32<TDevice>::Impl
	{
		// Internal implementation details can be added here if needed
	};
}