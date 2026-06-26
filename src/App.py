import config.Config as Config
from core.Serial import SerialManager
from core.PC import PC
from core.Controller import Controller
from tray.Tray import Tray
import wmi

import clr
clr.AddReference(r'../LibreHardwareMonitor/LibreHardwareMonitorLib')
from LibreHardwareMonitor.Hardware import Computer


class App:
    def __init__(self):
        self.pc = PC()

        self.serial = SerialManager(
            Config.SERIAL_PORT,
            Config.BAUD_RATE
        )

        self.wmi = wmi.WMI()

        self.hw = Computer()
        self.hw.IsCpuEnabled = True
        self.hw.IsMemoryEnabled = True
        self.hw.Open()

        self.controller = Controller(
            pc=self.pc,
            serial=self.serial,
            config=Config
        )

        self.tray = Tray(
            on_settings=self.on_settings,
            on_exit=self.on_exit
        )

    def on_settings(self):
        print("Abrir settings")

    def on_exit(self):
        print("Saindo...")
        self.serial.close()