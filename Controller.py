class Controller():
    def __init__(self):
        self.rpmCurve = [
            (50, 20),
            (60, 40),
            (70, 60),
            (80, 80),
            (90, 100)
        ]
        self.tempLimit = 3
        self.alpha_up = 0.7
        self.alpha_down = 0.2
        self.lastRPM = None
        self.currentStep = 0

    def setRPM(self, cpuTemperature):
        while self.currentStep < len(self.rpmCurve) - 1:
            nextTemp = self.rpmCurve[self.currentStep + 1][0]
            if cpuTemperature >= nextTemp:
                self.currentStep += 1
            else:
                break

        while self.currentStep > 0:
            currentTemp = self.rpmCurve[self.currentStep][0]
            if cpuTemperature < currentTemp - self.tempLimit:
                self.currentStep -= 1
            else:
                break

        return self.rpmCurve[self.currentStep][1]

    def smoothRPM(self, targetRPM):
        if self.lastRPM is None:
            self.lastRPM = float(targetRPM)
            return targetRPM

        if abs(targetRPM - self.lastRPM) < 2:
            return int(self.lastRPM)

        if targetRPM > self.lastRPM:
            alpha = self.alpha_up
        else:
            alpha = self.alpha_down

        smoothed = alpha * targetRPM + (1 - alpha) * self.lastRPM
        self.lastRPM = smoothed
        return int(smoothed)