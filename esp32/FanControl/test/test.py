import random
import serial
import struct
from time import sleep

ser = serial.Serial("/dev/ttyUSB0", 115200)

while True:
    temp = random.randint(1, 100)
    rpm = random.randint(1,100)
    load = random.randint(99,100)
    uram = int(113)
    tram = int(197)

    data = struct.pack("<BBBHH", temp, load, rpm, uram, tram)
    ser.write(b'\xAA')
    ser.write(data)
    print(f"Bytes enviados: {len(data)}")
    print("Dado enviado")
    sleep(1)


