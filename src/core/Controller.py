import struct

class Controller:
    def __init__(self, pc, serial, config):
        self.pc = pc
        self.serial = serial
        self.config = config

        self.rpmCurves = config.PROFILES
        self.curve = config.PROFILE
        self.tempLimit = config.TEMP_LIMIT
        self.alpha_up = config.ALPHA_UP
        self.alpha_down = config.ALPHA_DOWN

        self.lastRPM = None
        self.currentStep = 0

    def update(self, hw, wmi):

        temp = int(self.pc.getTemp(hw))
        load = int(self.pc.getLoad(hw))

        rpm = self.setRPM(temp)
        srpm = self.smoothRPM(rpm)

        tram, _, uram = self.pc.getRAM(wmi)

        data = struct.pack(
            "<BBBHH",
            temp,
            load,
            srpm,
            int(uram * 10),
            int(tram * 10)
        )

        info = (
            f"Temp: {temp}°C | "
            f"Load: {load}% | "
            f"RPM: {srpm}%"
        )

        return info, data

    def setRPM(self, cpuTemperature):
        while self.currentStep < len(self.rpmCurves[self.curve]) - 1:
            nextTemp = self.rpmCurves[self.curve][self.currentStep + 1][0]
            if cpuTemperature >= nextTemp:
                self.currentStep += 1
            else:
                break

        while self.currentStep > 0:
            currentTemp = self.rpmCurves[self.curve][self.currentStep][0]
            if cpuTemperature < currentTemp - self.tempLimit:
                self.currentStep -= 1
            else:
                break

        return self.rpmCurves[self.curve][self.currentStep][1]

    def smoothRPM(self, targetRPM):
        if self.lastRPM is None:
            self.lastRPM = float(targetRPM)
            return targetRPM

        if abs(targetRPM - self.lastRPM) < 2:
            return int(self.lastRPM)

        alpha = self.alpha_up if targetRPM > self.lastRPM else self.alpha_down

        smoothed = alpha * targetRPM + (1 - alpha) * self.lastRPM
        self.lastRPM = smoothed

        return int(smoothed)