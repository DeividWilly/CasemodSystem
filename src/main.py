import time
import struct
import serial
import threading
import wmi
import clr
clr.AddReference(r'../LibreHardwareMonitor/LibreHardwareMonitorLib')
from LibreHardwareMonitor.Hardware import Computer

wmi_Obj = wmi.WMI()
from Controller import Controller
from PC import PC

deviceSerial = serial.Serial("COM4", 115200, timeout=1)


def read_serial():
    while True:
        try:
            if deviceSerial.in_waiting:
                data = deviceSerial.readline().decode(errors='ignore').strip()

                if data:
                    print(f"[ESP32] {data}")

            else:
                time.sleep(0.01)

        except Exception as e:
            print("Erro leitura:", e)
            time.sleep(1)

def main_loop():
    pc = PC()
    s = Controller()
    LibHw = Computer()
    LibHw.IsCpuEnabled = True
    LibHw.Open()
    

    while True:

        try:
            temp = int(pc.getTemp(LibHw))
            load = int(pc.getLoad(LibHw))
            rpm = s.setRPM(temp)
            srpm = s.smoothRPM(rpm)
            uram = pc.getRAM(wmi_Obj)[2]
            tram = pc.getRAM(wmi_Obj)[0]

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

        print("Conectado ao LibreHardwareMonitor!")

        try:
            main_loop()
        except Exception as e:
            print("Falha geral:", e)

        print("Reconectando em 2s...\n")
        time.sleep(2)