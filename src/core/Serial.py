import serial
import time

class SerialManager:
    def __init__(self, port, baudrate, timeout=1):
        self.device = serial.Serial(
            port=port,
            baudrate=baudrate,
            timeout=timeout
        )

    def read_loop(self):
        while True:
            try:
                if self.device.in_waiting:
                    data = (
                        self.device
                        .readline()
                        .decode(errors="ignore")
                        .strip()
                    )

                    if data:
                        print(f"[ESP32] {data}")
                else:
                    time.sleep(0.01)

            except Exception as e:
                print("Read error:", e)
                time.sleep(1)

    def send(self, data):
        self.device.write(b"\xAA")
        self.device.write(data)

    def close(self):
        if self.device.is_open:
            self.device.close()