package pl.umamusume.yhkcatprint.utils

import kotlinx.datetime.*
import kotlinx.serialization.Serializable
import kotlinx.coroutines.*

@Serializable
class Printer(val message: String) {
    fun printMessage() = runBlocking {
        val now: Instant = Clock.System.now()
        launch {
            delay(1000L)
            println(now.toString())
        }
        println(message)
    }
}

class NativeTest() {
    init {
        System.load("D:/IdeaProjects/YHKCatPrint/utils/libs/YHKCatPrint/x64/Release/YHKCatPrint.dll")
    }

    external fun printHelloFromNative()
}