SERIAL_PORT = "COM4"
BAUD_RATE = 115200

# =================== CURVES ======================

PROFILE = 0
TEMP_LIMIT = 3
ALPHA_UP = 0.7
ALPHA_DOWN = 0.2

RPM_CURVE_STD = [
    (50, 20),
    (60, 40),
    (70, 60),
    (80, 80),
    (90, 100)
]

RPM_CURVE_AGRESSIVE = [
    (110, 100)
]

RPM_SURVE_SILENCE = [
    (50, 20),
    (60, 20),
    (70, 40),
    (80, 50),
    (90, 60)
]

PROFILES = [RPM_CURVE_STD, RPM_CURVE_AGRESSIVE, RPM_SURVE_SILENCE]