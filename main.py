from datetime import datetime        
import tkinter
from tkinter import tk
import argparse
import os
import numpy
import time
import smbus
import RPi.GPIO as GPIO
from constants import *
from tkinter.filedialog import askopenfilename






    

    
def draw(file):
    image = Image.open(file)
    width, height = image.size
    dimX = []
    dimY = []
    for x in range(0, width):
        for y in range(0, height):
            r, g, b = image.getpixel((x, y))
            if r  < 50 and g > r and g > b:
                dimX.append(x)
                dimY.append(y)
            y += 1
        x += 1
    ImageDraw.Draw(image).rectangle([min(dimX), min(dimY), max(dimX), max(dimY)], outline="red")
    image.save(DATE_TIME.strftime("%H-%M-%S_%d.%m.%Y") + ".ppm")

def microphoneDetection():
    try:
        while True:
            bus = smbus.SMBus(1)
            bus.write_byte(I2CADDRESS, 0x20)

            temp = bus.read_word_data(I2CADDRESS, 0x00)

            first = temp >> 8
            second = temp << 8

            switch = first | second
            comparison = switch & 0x0FFF

            if comparison > THRESHOLD:
                global DATE_TIME
                DATE_TIME = datetime.now()

                print("Microphone: Bug detected - " + DATE_TIME.strftime("%H:%M:%S %d/%m/%y"))

                global MICROPHONE_DETECTIONS
                MICROPHONE_DETECTIONS += 1
                break
    except KeyboardInterrupt:
        log("Microphone", DATE_TIME)

'''
def monitorLevels():
    for value in SOUND_CONTROL:
        GPIO.setup(value, GPIO.OUT)
    bus = smbus.SMBus(1)

    while True:
        bus.write_byte(I2CADDRESS, 0x20)

        temp = bus.read_word_data(I2CADDRESS, 0x00)

        first = temp >> 8
        second = temp << 8

        switch = first | second

        for i in range(len(SOUND_CONTROL)):
            if switch > THRESHOLD_VALUES[i]:
                GPIO.output(SOUND_CONTROL[i], True)
            else:
                GPIO.output(SOUND_CONTROL, False)
'''

def GUI():
    root = tkinter.Tk(className="Bug ID")
    cameraStatus = tkinter.Label(root, text="Camera: " + CAMERA_STATUS).pack()
    cameraDetections = tkinter.Label(root, text="Camera Detections: " + str(IMAGE_DETECTIONS)).pack()
    microphoneStatus = tkinter.Label(root, text="Microphone: " + MICROPHONE_STATUS).pack()
    microphoneDetections = tkinter.Label(root, text="Microphone Detections: " + str(MICROPHONE_DETECTIONS)).pack()
    systemState = tkinter.Label(root, text="System State: " + PWM_STATE).pack()
    PWM = tkinter.Button(root, text="Toggle PWM", action=togglePWM).pack()
    snap = tkinter.Button(root, text="Snap", action=captureImage(False)).pack()

    photo = ImageTk.PhotoImage(Image.open(DATE_TIME.strftime("%H-%M-%S_%d.%m.%Y") + ".ppm"))
    panel = tkinter.Label(root, image = photo).pack(side="bottom", fill="both", expand="yes")

    root.mainloop()
