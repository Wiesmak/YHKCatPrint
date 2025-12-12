#include "nativeprinter.h"
#include <iostream>
#include <memory>
#include "IDevice.h"
#include "ProtoDevice.h"
#include "IRfcommSocket.h"
#include "ProtoRfcommSocket.h"

using yhkcatprint::IRfcommSocket;
using yhkcatprint::ProtoRfcommSocket;
using yhkcatprint::IDevice;
using yhkcatprint::ProtoDevice;

JNIEXPORT void JNICALL Java_pl_umamusume_yhkcatprint_utils_NativePrinter_printBuffer(JNIEnv* env, jobject obj, jbyteArray buffer, jint length) {
	uint8_t* data = reinterpret_cast<uint8_t*>(env->GetByteArrayElements(buffer, nullptr));

	if (data == nullptr) {
		std::cerr << "Failed to get byte array elements." << std::endl;
		return;
	}

	jlong capacity = env->GetArrayLength(buffer);
	if (length > capacity) {
		std::cerr << "Size parameter exceeds buffer capacity." << std::endl;
		env->ReleaseByteArrayElements(buffer, reinterpret_cast<jbyte*>(data), 0);
		return;
	}

	auto device = std::make_unique<ProtoDevice>("24:00:28:00:1e:5b", "Cat Printer");
	auto socket = device->createRfcommSocket(2, yhkcatprint::ConnectOptions::TIMEOUT_NONE);


	try {
		socket->connect();
		const uint8_t initCmd[] = { 0x1b, 0x40 };
		socket->send(initCmd, sizeof(initCmd));
		// send \x1e\x47\x03 receive status 38 bytes
		const uint8_t getStatusCmd[] = { 0x1e, 0x47, 0x03 };
		socket->send(getStatusCmd, sizeof(getStatusCmd));
		uint8_t status[38];
		size_t received = socket->receive(status, sizeof(status));
		std::cout << "Received " << received << " bytes of status data." << std::endl;
		std::cout << "Status Data: ";
		// send \x1D\x67\x39 receive 21 bytes
		const uint8_t getSerialCmd[] = { 0x1D, 0x67, 0x39 };
		socket->send(getSerialCmd, sizeof(getSerialCmd));
		uint8_t serial[21];
		size_t serialReceived = socket->receive(serial, sizeof(serial));
		std::cout << "Received " << serialReceived << " bytes of serial number data." << std::endl;
		std::cout << "Serial Number Data: ";
		for (size_t i = 0; i < serialReceived; ++i) {
			std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(serial[i]) << " ";
		}
		// send start print sequence \x1d\x49\xf0\x19
		const uint8_t startPrintCmd[] = { 0x1d, 0x49, 0xf0, 0x19 };
		socket->send(startPrintCmd, sizeof(startPrintCmd));
		// send buffer
		socket->send(data, static_cast<size_t>(length));
		// send end print sequence \x0a\x0a\x0a\x0a
		const uint8_t endPrintCmd[] = { 0x0a, 0x0a, 0x0a, 0x0a };
		socket->send(endPrintCmd, sizeof(endPrintCmd));
		socket->close();
	}
	catch (const std::exception& ex) {
		std::cerr << "Error: " << ex.what() << std::endl;
		
		if (socket) {
			socket->close();
		}
	}
}