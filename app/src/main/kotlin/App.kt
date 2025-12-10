package pl.umamusume.yhkcatprint.app

import pl.umamusume.yhkcatprint.utils.NativePrinter

fun main() {
    val name = "Kotlin"
    val message = "Hello, $name!"
    val printer = NativePrinter(message)
    printer.print()

    for (i in 1..5) {
        println("i = $i")
    }
}
