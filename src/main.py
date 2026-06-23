from Config import *
from Struct import getData
from Serial import read_serial, send_data
from Tray import start_tray, update_info

import threading
import time


def main():

    threading.Thread(
        target=start_tray,
        daemon=True
    ).start()

    threading.Thread(
        target=read_serial,
        daemon=True
    ).start()

    while True:

        info, data = getData(
            PC_OBJ,
            CONTROLLER_OBJ,
            HW_OBJECT,
            WMI_OBJ
        )

        send_data(data)

        update_info(info)

        print(info)

        time.sleep(1)


if __name__ == "__main__":
    main()