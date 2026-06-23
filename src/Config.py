import serial
import wmi
import clr

from Controller import Controller
from PC import PC

clr.AddReference(r'../LibreHardwareMonitor/LibreHardwareMonitorLib')
from LibreHardwareMonitor.Hardware import Computer

SERIAL_PORT = "COM4"
BAUD_RATE = 115200

WMI_OBJ = wmi.WMI()

DEVICE_SERIAL = serial.Serial(
    SERIAL_PORT,
    BAUD_RATE,
    timeout=1
)

HW_OBJECT = Computer()
HW_OBJECT.IsCpuEnabled = True
HW_OBJECT.IsMemoryEnabled = True
HW_OBJECT.Open()

PC_OBJ = PC()
CONTROLLER_OBJ = Controller()