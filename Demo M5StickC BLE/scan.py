from bleak import BleakScanner
import asyncio

async def async_scan():
    device = await BleakScanner.find_device_by_filter(
        lambda dev, adv: dev.name and "m5" in dev.name.lower()
    )
    if device:
        print(device.name, device.address)
    else:
        print("Device Not Found")

if __name__ == '__main__':
    asyncio.run(async_scan())