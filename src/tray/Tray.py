import pystray
import os
from PIL import Image

BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
IMAGE_PATH = os.path.join(BASE_DIR, "assets", "logo.png")

class Tray:
    def __init__(self, on_settings=None, on_exit=None):
        self.icon = None
        self.on_settings = on_settings
        self.on_exit = on_exit

        self.image = Image.open(IMAGE_PATH).resize(
            (128, 128),
            Image.LANCZOS
        )

    def _on_click(self, icon, item):
        if item.text == "Settings" and self.on_settings:
            self.on_settings()

        elif item.text == "Exit":
            self.stop()

    def start(self):
        self.icon = pystray.Icon(
            "ESP32",
            self.image,
            "Initializing...",
            menu=pystray.Menu(
                pystray.MenuItem("Settings", self._on_click),
                pystray.MenuItem("Exit", self._on_click)
            )
        )

        self.icon.run()

    def update_info(self, text):
        if self.icon:
            self.icon.title = text

    def stop(self):
        if self.icon:
            self.icon.stop()

        if self.on_exit:
            self.on_exit()

        os._exit(0)