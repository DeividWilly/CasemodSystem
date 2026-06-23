import time
from Config import DEVICE_SERIAL

def read_serial():

    while True:
        try:
            if DEVICE_SERIAL.in_waiting:

                data = (
                    DEVICE_SERIAL
                    .readline()
                    .decode(errors='ignore')
                    .strip()
                )

                if data:
                    print(f"[ESP32] {data}")

            else:
                time.sleep(0.01)

        except Exception as e:
            print("Read error:", e)
            time.sleep(1)


def send_data(data):

    DEVICE_SERIAL.write(b'\xAA')
    DEVICE_SERIAL.write(data)