/*
 * Copyright (c) 2026. Umamusume Polska
 */

package pl.umamusume.yhkcatprint.bluetooth

import io.mockk.Runs
import io.mockk.every
import io.mockk.just
import io.mockk.mockk
import io.mockk.unmockkAll
import io.mockk.verify
import kotlinx.coroutines.ExperimentalCoroutinesApi
import kotlinx.coroutines.launch
import kotlinx.coroutines.runBlocking
import kotlinx.coroutines.test.advanceUntilIdle
import kotlinx.coroutines.test.runTest
import java.io.IOException
import kotlin.test.AfterTest
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

    @AfterTest
    fun teardown() {
        unmockkAll()
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

    @Test
    fun testResumeOnError() = runBlocking {
        every { rfcommSocketGateway.connect(2137L, 5000 ,any()) } answers {
            val callback = thirdArg<RfcommSocket.ConnectionCallback>()
            callback.onError(RuntimeException("Connection failed"))
        }

        try {
            socket.connect()
        } catch (e: Exception) {
            assertEquals("Connection failed", e.message)
        }

        verify { rfcommSocketGateway.connect(2137L, 5000, any()) }
    }

    @OptIn(ExperimentalCoroutinesApi::class)
    @Test
    fun testCancelConnectOnCancellation() = runTest {
        every {
            rfcommSocketGateway.connect(2137L, any() ,any())
        } just Runs

        every {
            rfcommSocketGateway.cancelConnect(2137L)
        } returns Unit

        val job = launch {
            socket.connect(timeoutMs = 5000)
        }

        advanceUntilIdle()

        job.cancel()

        advanceUntilIdle()

        verify { rfcommSocketGateway.cancelConnect(2137L) }
    }

    @Test
    fun testWriteDelegatesToGateway() = runTest {
        val dataToWrite = byteArrayOf(0x0A, 0x0B, 0x0C)

        every { rfcommSocketGateway.write(2137L, dataToWrite) } returns Unit

        socket.write(dataToWrite)

        verify(exactly = 1) { rfcommSocketGateway.write(2137L, dataToWrite) }
    }

    @Test
    fun testWriteExceptionPropagation() = runTest {
        val dataToWrite = byteArrayOf(0x0A, 0x0B, 0x0C)
        val expectedError = IOException("Write failed")

        every { rfcommSocketGateway.write(2137L, dataToWrite) } throws expectedError

        try {
            socket.write(dataToWrite)
        } catch (e: Exception) {
            assertEquals("Write failed", e.message)
        }

        verify(exactly = 1) { rfcommSocketGateway.write(2137L, dataToWrite) }
    }
}