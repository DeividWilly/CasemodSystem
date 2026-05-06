import time
import struct
import serial
import threading
import requests

from Controller import Controller
from PC import PC

deviceSerial = serial.Serial("COM3", 115200, timeout=1)

url = "http://localhost:8085/data.json"

def verifyApp():
    try:
        r = requests.get(url, timeout=1)
        return r.status_code == 200
    except:
        return False

def read_serial():
    while True:
        try:
            if deviceSerial.in_waiting:
                data = deviceSerial.readline().decode(errors='ignore').strip()
                print(f"[ESP32] {data}")
        except Exception as e:
            print("Erro leitura:", e)

def main_loop():
    pc = PC()
    s = Controller()

    while True:
        try:
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

        except Exception as e:
            print("Erro no loop principal:", e)
            break  # sai pra reconectar

if __name__ == "__main__":

    threading.Thread(target=read_serial, daemon=True).start()

    while True:
        print("Aguardando LibreHardwareMonitor...")

        while not verifyApp():
            time.sleep(1)

        print("Conectado ao LibreHardwareMonitor!")

        try:
            main_loop()
        except Exception as e:
            print("Falha geral:", e)

        print("Reconectando em 2s...\n")
        time.sleep(2)