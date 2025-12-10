#include <iostream>
#include "nativetest.h"

JNIEXPORT void JNICALL Java_pl_umamusume_yhkcatprint_utils_NativeTest_printHelloFromNative(JNIEnv* env, jobject obj) {
    std::cout << "Hello from Native C++!" << std::endl;
}