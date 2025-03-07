# Import necessary libraries
import platform               # Used to determine platform (e.g., Windows, Linux)
import logging                # For logging purposes
import asyncio                # To manage asynchronous operations
from bleak import BleakClient # A class from the bleak library for BLE communication
from bleak import _logger as logger  # Logger for bleak library
from bleak.uuids import uuid16_dict  # A dictionary containing commonly used UUIDs for BLE services

# Define Nordic Uart Service (NUS) characteristic UUIDs
UART_RX_UUID = "6e400002-b5a3-f393-e0a9-e50e24dcca9e" #Nordic NUS characteristic for RX
UART_TX_UUID = "6e400003-b5a3-f393-e0a9-e50e24dcca9e" #Nordic NUS characteristic for TX

# Notification handler function, prints received data and sets dataFlag to True
def notification_handler(sender, data):
    """Simple notification handler which prints the data received."""
    print(data)


# Asynchronous function to run the BLE communication
async def run_func(address):
    # Establish BLE connection with the specified address
    async with BleakClient(address) as client:

        # Check if the BLE client is connected
        x = await client.is_connected()
        print("Connected: {0}".format(x))

        # Start notifying for data on the RX characteristic
        await client.start_notify(UART_TX_UUID, notification_handler)

        # Continuous loop for processing data
        while True : 

            #give some time to do other tasks
            await asyncio.sleep(0)


if __name__ == "__main__":

    #this is MAC of our BLE device
    address = (
        # "E8:5C:50:40:25:28"                               # สำหรับตัวมีจอ
        # "EF:5C:83:9D:35:9D"                               # สำหรับตัวเล็ก
        "D8:A0:1D:56:4B:AA"                                 # M5StickC
    )

    asyncio.run(run_func(address))