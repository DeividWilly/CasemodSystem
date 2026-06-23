import pystray
import os
from PIL import Image

image = Image.open("../assets/logo.png").resize((128, 128), Image.LANCZOS)
icon = None


def icon_stop():
    global icon

    if icon:
        icon.stop()

    os._exit(0)


def after_click(icon, item):
    if str(item) == "Exit":
        icon_stop()


def start_tray():

    global icon

    icon = pystray.Icon(
        "ESP32",
        image,
        "Initializing...",
        menu=pystray.Menu(
            pystray.MenuItem("Exit", after_click)
        )
    )

    icon.run()


def update_info(text):

    global icon

    if icon:
        icon.title = text