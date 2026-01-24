/*
 * Copyright (c) 2026. Umamusume Polska
 */

package pl.umamusume.yhkcatprint.bluetooth

import io.mockk.every
import io.mockk.mockk
import io.mockk.verify
import kotlinx.coroutines.runBlocking
import kotlin.test.BeforeTest
import kotlin.test.Test
import kotlin.test.assertEquals


class RfcommSocketTest {
    val mockGateway = mockk<BluetoothNativeGateway>()
    lateinit var rfcommSocketGateway: BluetoothNativeGateway.RfcommSocketGateway
    lateinit var socket: RfcommSocket

    @BeforeTest
    fun setup() {
        rfcommSocketGateway = mockk()
        every { mockGateway.rfcommSocket } returns rfcommSocketGateway
        socket = RfcommSocket(2137L, mockGateway)
    }

    @Test
    fun testToString() {
        val expected = "RfcommSocket(ptr=2137)"
        assertEquals(expected, socket.toString())
    }

    @Test
    fun testResumeOnSuccess() = runBlocking {
        every { rfcommSocketGateway.connect(2137L, 5000 ,any()) } answers {
            val callback = thirdArg<RfcommSocket.ConnectionCallback>()
            callback.onSuccess()
        }

        socket.connect()

        verify { rfcommSocketGateway.connect(2137L, 5000, any()) }
    }
}