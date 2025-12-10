package pl.umamusume.yhkcatprint.utils

import kotlinx.datetime.*
import kotlinx.serialization.Serializable
import kotlinx.coroutines.*

@Serializable
class NativePrinter(val message: String) {
    init {
        System.load("D:/IdeaProjects/YHKCatPrint/utils/libs/YHKCatPrint/x64/Release/YHKCatPrint.dll")
    }

    fun print() {
        printMessage(message)
    }

    private external fun printMessage(message: String);
}