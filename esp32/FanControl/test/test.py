import random
import serial
import struct
from time import sleep

ser = serial.Serial("/dev/ttyUSB0", 115200)

while True:
    temp = random.randint(1, 120)
    rpm = random.randint(1,100)
    load = random.randint(1,100)

    data = struct.pack("<BBB", temp, load, rpm)
    
    ser.write(data)
    print("Dado enviado")

    sleep(1)


