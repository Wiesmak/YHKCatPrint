#pragma once
#include <jni.h>

#ifndef _NATIVEPRINTER_H
#define _NATIVEPRINTER_H

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

	JNIEXPORT void JNICALL Java_pl_umamusume_yhkcatprint_utils_NativePrinter_printBuffer(JNIEnv* env, jobject obj, jbyteArray buffer, jint length);

#ifdef __cplusplus
}
#endif

#endif // !_NATIVEPRINTER_H

