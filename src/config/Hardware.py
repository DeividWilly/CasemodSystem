import serial
import wmi
import clr

from config import Config

clr.AddReference(r'../LibreHardwareMonitor/LibreHardwareMonitorLib')
from LibreHardwareMonitor.Hardware import Computer

WMI_OBJ = wmi.WMI()

DEVICE_SERIAL = serial.Serial(
    Config.SERIAL_PORT,
    Config.BAUD_RATE,
    timeout=1
)

HW_OBJECT = Computer()
HW_OBJECT.IsCpuEnabled = True
HW_OBJECT.IsMemoryEnabled = True
HW_OBJECT.Open()