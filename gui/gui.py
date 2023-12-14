import tkinter
import cv2
import PIL.Image, PIL.ImageTk
import time
import bleak
import asyncio
import keyboard
from bleak import BleakScanner, BleakClient
import argparse
import threading
import os
import functools
import logging

DEVICE_NAME = "ece453-group04"

BUTTON_SERVICE = 'c75393c3-c849-46b7-b5df-6337cefa2d9b'
BUTTON_CHARACTERISTIC = 'dab7603a-f4c7-47e4-9c06-7927c469402e'

LED_SERVICE = 'c2c231d0-d55f-43ec-b3a0-5ac2ae8cc14b'
LED_CHARACTERISTIC = '11FE0DE3-2073-431E-AD22-961699336339' 

MOTOR_SERIVCE = '78cb30ed-868a-4b41-86d3-74656a891457'
MOTOR_CHARACTERISTIC = 'fe13605d-1b80-4664-bbbb-e11f0477a4b2'


TEMP_SERVICE = '3e719f3a-9df8-44f4-9783-21d264a43e48'
TEMP_CHARACTERISTIC = 'daa969d3-4437-4dd9-bc40-1a38fc9578da'

PUMP_SERVICE = '0ec8d260-117f-4ab3-b1bb-ce7e74583fd5'
PUMP_CHARACTERISTIC = '2738b799-bd3f-4660-b20d-609d75bfb6a6'

BUZZER_SERVICE = '09d72c76-2d7e-4c7e-a6cf-7b819cdc5af8'
BUZZ_CHARACTERISTIC = '95b59b9d-117c-4898-9331-ea8f201d7000' 


# Debug mode
os.environ["PYTHONASYNCIODEBUG"] = "1"

#switch video_source to the fifo264 pipe to integrate with the raspberry pi camera
class App:
    def __init__(self, window, window_title, video_source=0):

        self.window = window
        self.window.title(window_title)
        self.video_source = video_source
        self.connected = False
        self.moving = False
        self.count = 0
        self.status_color = "red" 
        self.width = 500
        self.height = 200

        self.pre_dir = None
        self.curr_dir = None 

        # connect to PSoc6 
        asyncio.ensure_future(self.connect(DEVICE_NAME))

        # open video source (by default this will try to open the computer webcam)
        self.vid = MyVideoCapture(self.video_source)

        # Create a canvas that can fit the above video source size
        self.canvas = tkinter.Canvas(window, width = self.width, height = self.height)
        self.canvas.pack()

        # Get key presses
        self.window.bind("<Key>", self.on_key_press)

        # initialize pump
        self.pump = False

        # Button that lets the user move Bogo
        self.btn1=tkinter.Button(window, text="W", width=3, bg="light blue")
        self.btn1.place(x=self.width/8, y=self.height-100)

        self.btn2=tkinter.Button(window, text="S", width=3, bg="light blue")
        self.btn2.place(x=self.width/8, y=self.height-50)

        self.btn3=tkinter.Button(window, text="A", width=3, bg="light blue")
        self.btn3.place(x=(self.width/8)-50, y=self.height-50)

        self.btn4=tkinter.Button(window, text="D", width=3, bg="light blue")
        self.btn4.place(x=(self.width/8)+50, y=self.height-50)

        self.btn5=tkinter.Button(window, text="Raise Hose (J)", width=10, bg="light blue")
        self.btn5.place(x=self.width-125, y=self.height-100)

        self.btn6=tkinter.Button(window, text="Lower Hose (K)", width=10, bg="light blue")
        self.btn6.place(x=self.width-125, y=self.height-50)

        self.btn7=tkinter.Button(window, text="Spray Water ( )", width=15, bg="orange")
        self.btn7.place(x=(self.width/2)-50, y=self.height-50)

        # display temperature
        self.status_temp = 'red'
        self.temp_label = tkinter.Label(window, text="temp", bg=self.status_temp)
        self.temp_label.place(x=0, y=0)
        self.temperature = 'DEVICE NOT CONNECTED'

        # display proximity
        self.prox_label = tkinter.Label(window, text="proximity", bg=self.status_color)
        self.prox_label.place(x=0, y=20)
        self.proximity = 'DEVICE NOT CONNECTED'
        self.status_color = 'red'
         

    def on_key_press(self, event):

        self.prev_dir = self.curr_dir
        self.curr_dir = event
        
        if (self.prev_dir == None or self.curr_dir.char != self.prev_dir.char):
            if (self.curr_dir.char == 'w'):
                #if "Front" not in self.proximity:
                asyncio.ensure_future(self.client.write_gatt_char(MOTOR_CHARACTERISTIC, b'00', response=False))
                self.moving = True
                
            elif (self.curr_dir.char == 's'):
                #if "Back" not in self.proximity:
                asyncio.ensure_future(self.client.write_gatt_char(MOTOR_CHARACTERISTIC, b'10', response=False))
                self.moving = True

            elif (self.curr_dir.char == 'a'):
                #if "Left" not in self.proximity:
                asyncio.ensure_future(self.client.write_gatt_char(MOTOR_CHARACTERISTIC, b'20', response=False))
                self.moving = True

            elif (self.curr_dir.char == 'd'):
                #if "Right" not in self.proximity:
                asyncio.ensure_future(self.client.write_gatt_char(MOTOR_CHARACTERISTIC, b'30', response=False))
                self.moving = True

            elif (self.curr_dir.char == 'q'):
                asyncio.ensure_future(self.client.write_gatt_char(MOTOR_CHARACTERISTIC, b'40', response=False))
                self.moving = False

        if (self.curr_dir.char == 'j'):
            asyncio.ensure_future(self.client.write_gatt_char(PUMP_CHARACTERISTIC, b'20', response=False))

        elif (self.curr_dir.char == 'k'):
            asyncio.ensure_future(self.client.write_gatt_char(PUMP_CHARACTERISTIC, b'30', response=False))

        if (self.curr_dir.char == ' '):
            if (self.pump):
                asyncio.ensure_future(self.client.write_gatt_char(PUMP_CHARACTERISTIC, b'00', response=False))

            else:
                asyncio.ensure_future(self.client.write_gatt_char(PUMP_CHARACTERISTIC, b'10', response=False))

            self.pump = not self.pump

    def check_prox(self):
        if self.moving == True:
            if self.curr_dir.char == 'w' and 'Front' in self.proximity:
                asyncio.ensure_future(self.client.write_gatt_char(MOTOR_CHARACTERISTIC, b'40', response=False))
            if self.curr_dir.char == 's' and 'Back' in self.proximity:
                asyncio.ensure_future(self.client.write_gatt_char(MOTOR_CHARACTERISTIC, b'40', response=False))
            if self.curr_dir.char == 'a' and 'Left' in self.proximity:
                asyncio.ensure_future(self.client.write_gatt_char(MOTOR_CHARACTERISTIC, b'40', response=False))
            if self.curr_dir.char == 'd' and 'Right' in self.proximity:
                asyncio.ensure_future(self.client.write_gatt_char(MOTOR_CHARACTERISTIC, b'40', response=False))
    
    def set_prox(self, proximity):
        self.proximity = proximity
        if "Warning" in proximity:
            self.status_color = 'red'
        else:
            self.status_color = 'green'

    def set_temp(self, temperature):
        self.temperature = temperature
        self.status_temp = 'green'

    def update(self):
        # Get a frame from the video source
        ret = False
        ret, frame = self.vid.get_frame()
        if self.connected and not self.pump:
            if (self.count >= 10):
                fut_temp = asyncio.ensure_future(self.client.read_gatt_char(TEMP_CHARACTERISTIC))
                fut_temp.add_done_callback(set_temp)
                fut_prox = asyncio.ensure_future(self.client.read_gatt_char(BUTTON_CHARACTERISTIC))
                fut_prox.add_done_callback(set_prox_handle)
                self.count = 0
        
            self.count += 1
        self.check_prox() 
        if ret:
            self.temp_label.config(text=self.temperature, bg=self.status_temp)
            self.prox_label.config(text=self.proximity, bg=self.status_color)
            self.window.update()

    async def connect(self, device_name):
        devices = await BleakScanner.discover()
        for d in devices:
            if d.name == device_name:
                psoc6 = d 
        if psoc6 == None:
            exit("Reset the PSoc6.")
        self.client = BleakClient(psoc6.address)
        try: 
            await self.client.connect()
            self.connected = await self.client.is_connected()
            print(self.connected)
            self.status_color = "green"
            self.status_temp = "green"
        except:
            exit()

    # working interval 0.05
    async def run_tk(self, interval=0.05):
        try:
            while True:
                self.update()
                await asyncio.sleep(interval)
        except tkinter.TclError as e:
            if "application has been destored" not in e.args[0]:
                raise
        except AttributeError as a:
            print(a)


class MyVideoCapture:
    def __init__(self, video_source=0):
        # Open the video source
        self.vid = cv2.VideoCapture(video_source)
        if not self.vid.isOpened():
            raise ValueError("Unable to open video source", video_source)

        # Get video source width and height
        self.width = self.vid.get(cv2.CAP_PROP_FRAME_WIDTH)
        self.height = self.vid.get(cv2.CAP_PROP_FRAME_HEIGHT)

    def get_frame(self):
        if self.vid.isOpened():
            ret, frame = self.vid.read()
            if ret:
                # Return a boolean success flag and the current frame converted to BGR
                return (ret, cv2.cvtColor(frame, cv2.COLOR_BGR2RGB))
            else:
                return (ret, None)
        else:
            return (ret, None)

    # Release the video source when the object is destroyed
    def __del__(self):
        if self.vid.isOpened():
            self.vid.release()

def set_temp(future):
    byte_array = future.result()
    bits_string = ''.join(format(byte, '08b') for byte in byte_array)
    integer_representation = int(bits_string[:-1], 2)
    if bits_string[-1] == '1':
        integer_representation += 0.5
    integer_representation = f'TEMPERATURE: {integer_representation} C'
    print(integer_representation)
    app.set_temp(integer_representation)

def set_prox_handle(future):
    # 1 = all clear; 0 = warning message
    byte_array = future.result()
    bits_string = ''.join(format(byte, '08b') for byte in byte_array)
    bits_string = bits_string[0:4]
    print(bits_string)
    prox_msg = ''    
    if '0' not in bits_string:
        prox_msg = "All Clear!"
        app.set_prox(prox_msg)
        return

    prox_msg += "Warning!" 
    if bits_string[3] == '0':
        prox_msg += " Front"
    if bits_string[2] == '0':
        prox_msg += " Left"
    if bits_string[0] == '0':
        prox_msg += " Right"
    if bits_string[1] == '0':
        prox_msg += " Back"

    app.set_prox(prox_msg)

async def main():
    await app.run_tk()

if __name__ == '__main__':
    app = App(tkinter.Tk(), "Tkineter and OpenCV", 0)
    asyncio.get_event_loop().run_until_complete(main())

