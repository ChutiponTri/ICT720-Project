from bleak import BleakScanner
import asyncio

async def scanner():
    device = await BleakScanner.find_device_by_filter(
        lambda dev, adv: dev.name and "m5stickc" in dev.name.lower()
    )

    if device:
        print(device.name, ":", device.address)
    else:
        print("Cannot Find")

if __name__ == '__main__':
    asyncio.run(scanner())