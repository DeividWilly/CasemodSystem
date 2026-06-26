import wmi
import clr

class PC():

    def __init__(self):
        self.usedRAM = 0
        self.freeRAM = 0
        self.totalRAM = 0

        self.temp = 0
        self.lastTemp = 0
        self.load = 0.1

    def getRAM(self, pc_obj):

        os = pc_obj.Win32_OperatingSystem()[0]

        totalRAM_kb = int(os.TotalVisibleMemorySize)
        freeRAM_kb = int(os.FreePhysicalMemory)
        usedRAM_kb = totalRAM_kb - freeRAM_kb

        self.totalRAM = totalRAM_kb / (1024 ** 2)
        self.freeRAM = freeRAM_kb / (1024 ** 2)
        self.usedRAM = usedRAM_kb / (1024 ** 2)

        return self.totalRAM, self.freeRAM, self.usedRAM

    def getTemp(self, obj):
        for hardware in obj.Hardware:

            hardware.Update()

            for sensor in hardware.Sensors:
                if sensor.SensorType.ToString() == "Temperature" and sensor.Name == "CPU Package":
                    self.temp = sensor.Value
        
        return self.temp

    def getLoad(self, obj):
        for hardware in obj.Hardware:

            hardware.Update()

            for sensor in hardware.Sensors:
                if sensor.SensorType.ToString() == "Load" and sensor.Name == "CPU Total":
                    self.load = sensor.Value
        return self.load

