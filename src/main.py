from App import App
import threading
import time


def main():
    app = App()

    serial_thread = threading.Thread(
        target=app.serial.read_loop,
        daemon=True
    )
    serial_thread.start()

    tray_thread = threading.Thread(
        target=app.tray.start
    )
    tray_thread.start()

    try:
        while True:
            info, data = app.controller.update(app.hw, app.wmi)

            app.serial.send(data)
            app.tray.update_info(info)

            print(info)
            time.sleep(1)

    except KeyboardInterrupt:
        print("Encerrando aplicação...")
        app.serial.close()
        app.tray.stop()


if __name__ == "__main__":
    main()