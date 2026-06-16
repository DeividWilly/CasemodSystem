import wmi 
import requests
import json

class PC():
    def __init__(self):
        self.usedRAM = 0
        self.freeRAM = 0
        self.totalRAM = 0
        self.temp = 0
        self.lastTemp = 0
        self.load = 0.1
        
    def getRAM(self):
        pc = wmi.WMI()
        os = pc.Win32_OperatingSystem()[0]
        
        totalRAM_kb = int(os.TotalVisibleMemorySize)
        freeRAM_kb = int(os.FreePhysicalMemory)
        usedRAM_kb = totalRAM_kb - freeRAM_kb
        
        self.totalRAM = totalRAM_kb / (1024 ** 2)
        self.freeRAM = freeRAM_kb / (1024 ** 2)
        self.usedRAM = usedRAM_kb / (1024 ** 2)

        return self.totalRAM, self.freeRAM, self.usedRAM
        
    def getTemp(self, api_url):
        response = requests.get(api_url)
        if response.status_code == 200:
            data = response.json()
            self.temp = int(data["Children"][0]["Children"][1]["Children"][3]["Children"][10]["Value"].replace(".0 °C", ""))
            return self.temp
            
        else:
            print("ERROR")
            return None
            
    def getLoad(self, api_url):
        response = requests.get(api_url)
        if response.status_code == 200:
            data = response.json()
            self.load = float(data["Children"][0]["Children"][1]["Children"][4]["Children"][0]["Value"].replace(" %", ""))
            return self.load
        else:
            print("ERROR")
            return None