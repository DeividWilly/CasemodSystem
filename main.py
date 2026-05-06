import sys
import time
import wmi
import struct
import serial
import threading
from Controller import Controller
from PC import PC

deviceSerial = serial.Serial("COM3", 115200, timeout=1)

port = "8085"
url = f"http://localhost:{port}/data.json"
path = r"LibreHardwareMonitor\LibreHardwareMonitor.exe"

def verifyApp():
    pc = wmi.WMI()
    for process in pc.Win32_Process():
        if "LibreHardwareMonitor.exe" == process.Name:
            return True
    return False

def read_serial():
    while True:
        try:
            if deviceSerial.in_waiting:
                data = deviceSerial.readline().decode(errors='ignore').strip()
                print(f"[ESP32] {data}")
        except Exception as e:
            print("Erro leitura:", e)

if __name__ == "__main__": 
    if verifyApp():
        print("\n" * 3)

        pc = PC()
        s = Controller()

        threading.Thread(target=read_serial, daemon=True).start()

        while True:
            temp = int(pc.getTemp(url))
            load = int(pc.getLoad(url))
            rpm = s.setRPM(temp)
            srpm = s.smoothRPM(rpm)
            uram = pc.getRAM()[2]
            tram = pc.getRAM()[0]

            data = struct.pack("<BBBHH", 
                temp, 
                load, 
                srpm, 
                int(uram * 10), 
                int(tram * 10)
            )

            deviceSerial.write(b'\xAA')
            deviceSerial.write(data)

            print(f"TX -> temp:{temp} load:{load} rpm:{srpm}")

            time.sleep(1)
    else:
        print("erro")
        print("erro")