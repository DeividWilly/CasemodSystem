import sys
import time
import wmi
import struct
# import serial
from Controller import Controller
from PC import PC

# deviceSerial = serial.Serial(f"{esp32}", 115200)
port = "8085"
url = str(f"http://localhost:{port}/data.json")
path = str(r"LibreHardwareMonitor\LibreHardwareMonitor.exe")

def verifyApp():
    pc = wmi.WMI()
    
    for process in pc.Win32_Process():
        if "LibreHardwareMonitor.exe" == process.Name:
            return True
    return False
        
if __name__ == "__main__": 
    if verifyApp() == True:
        print("\n" * 3)

        pc = PC()
        s = Controller()

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
                               ) # depois, tram dividido por 10.0
            print(int(uram*10))
            print(int(tram*10))
            print((int(tram*10))/10.0)
            time.sleep(1)
                               
    else:
        print("erro")