import bleak
import asyncio
from bleak import BleakScanner, BleakClient
import time
import binascii

BUTTON_SERVICE = 'c75393c3-c849-46b7-b5df-6337cefa2d9b'
BUTTON_CHARACTERISTIC = 'dab7603a-f4c7-47e4-9c06-7927c469402e'

LED_SERVICE = 'c2c231d0-d55f-43ec-b3a0-5ac2ae8cc14b'
LED_CHARACTERISTIC = '11FE0DE3-2073-431E-AD22-961699336339' 

MOTOR_SERIVCE = '78cb30ed-868a-4b41-86d3-74656a891457'
MOTOR_CHARACTERISTIC = 'fe13605d-1b80-4664-bbbb-e11f0477a4b2'

TEMP_SERVICE = '3e719f3a-9df8-44f4-9783-21d264a43e48'
TEMP_CHARACTERISTIC = 'daa969d3-4437-4dd9-bc40-1a38fc9578da'

PUMP_SERIICE = '0ec8d260-117f-4ab3-b1bb-ce7e74583fd5'
PUMP_CHARACTERISTIC = '2738b799-bd3f-4660-b20d-609d75bfb6a6'

async def main():
    psoc6 = None
    devices = await BleakScanner.discover()
    for d in devices:
        if d.name == "ece453-group04":
            psoc6 = d 
    if psoc6 == None:
        exit("Reset the PSoc6.")

    print(psoc6.address)
    async with BleakClient(psoc6.address) as client:
        await client.connect()
        connected = await client.is_connected()
        print(connected)

        #services = await client.get_services()
        #for s in services:
        #    print(s)
        #    for c in s.characteristics:
        #        print(f"\t{c}")

        #while True:
        #    temperature = await client.read_gatt_char(BUTTON_CHARACTERISTIC)
        #    print(temperature)
        #    user_input = input()
        #    if user_input == 'q':
        #        break
            
        #while True:
        #   await client.write_gatt_char(PUMP_CHARACTERISTIC, b'00', response=False) 
        #   user_input = input()
        #   if user_input == 'q':
        #        break
        #button_service = None
        #for s in services:
        #    if s.uuid == BUTTON_SERVICE: 
        #        button_service = s
        #if button_service == None:
        #    exit("Button Service not found")
        #for c in button_service.characteristics:
        #    print(c)

        while True:
            raw_temperature = await client.read_gatt_char(TEMP_CHARACTERISTIC)
            user_input = input()
            # Sample 8-bit temperature value

            # Scaling and offset factors
            scaling_factor = 0.5  # Each unit of 8-bit value represents 0.5 degrees
            offset = -10.0  # Offset to adjust the temperature

            # Convert the 8-bit value to temperature in Celsius
            hex_representation = binascii.hexlify(raw_temperature).decode()
            print(hex_representation)
            if user_input == 'q':
                break

        #prev_event = None
        #curr_event = None
        #while True:
        #    prev_event = curr_event
        #    curr_event = keyboard.read_event()
        #    
        #    if curr_event.event_type == keyboard.KEY_DOWN:
        #        if prev_event == None or prev_event.event_type != keyboard.KEY_DOWN:
        #            if curr_event.name == 'w':
        #                #print("w on")
        #                await client.write_gatt_char(LED_CHARACTERISTIC, b'00', response=False)
        #            elif curr_event.name == 's':
        #                #print("s on")
        #                await client.write_gatt_char(LED_CHARACTERISTIC, b'10', response=False)
        #            elif curr_event.name == 'a':
        #                #print("a on")
        #                await client.write_gatt_char(LED_CHARACTERISTIC, b'20', response=False)
        #            elif curr_event.name == 'd':
        #                #print("d on")
        #                await client.write_gatt_char(LED_CHARACTERISTIC, b'30', response=False)
        #            elif curr_event.name == 'q':
        #                await client.write_gatt_char(LED_CHARACTERISTIC, b'40', response=False)
        #                break
        #    elif curr_event.event_type == keyboard.KEY_UP:
        #        if prev_event.event_type != keyboard.KEY_UP:
        #            await client.write_gatt_char(LED_CHARACTERISTIC, b'40', response=False)
            
        await client.disconnect()

asyncio.run(main())
