#include <jni.h>

#ifndef _NATIVETEST_H
#define _NATIVETEST_H
#ifdef __cplusplus
extern "C" {
#endif

	JNIEXPORT void JNICALL Java_pl_umamusume_yhkcatprint_utils_NativeTest_printHelloFromNative(JNIEnv* env, jobject obj);

#ifdef __cplusplus
}

#endif
#endif // _INCLUDE_TESTNATIVE_H