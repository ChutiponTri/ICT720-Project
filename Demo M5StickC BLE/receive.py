from bleak import BleakClient
import asyncio            

# Define Nordic Uart Service (NUS) characteristic UUIDs
UART_RX_UUID = "6e400002-b5a3-f393-e0a9-e50e24dcca9e" #Nordic NUS characteristic for RX
UART_TX_UUID = "6e400003-b5a3-f393-e0a9-e50e24dcca9e" #Nordic NUS characteristic for TX
NAME_UUID = "00002a00-0000-1000-8000-00805f9b34fb"

def notification_handler(sender, data):
    """Simple notification handler which prints the data received."""
    print(data)

# Asynchronous function to run the BLE communication
async def run_func(address):
    async with BleakClient(address) as client:

        x = client.is_connected
        hex = await client.read_gatt_char(NAME_UUID)
        name = hex.decode()
        print("%s Connected: %s" % (name, x))

        # Start notifying for data on the RX characteristic
        await client.start_notify(UART_TX_UUID, notification_handler)

        # Continuous loop for processing data
        while True : 
            #give some time to do other tasks
            await asyncio.sleep(5)

            message = "Hello"
            await client.write_gatt_char(UART_RX_UUID, message.encode())


if __name__ == "__main__":
    #this is MAC of our BLE device
    address = "D8:A0:1D:56:4B:AA"                               # M5StickC
    address = "D8:A0:1D:46:34:0E"
    # address = "EF:5C:83:9D:35:9D"                               # XIAO MACKIE

    asyncio.run(run_func(address))