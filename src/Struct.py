import struct

def getData(pc, controller, hw, wmi):

    temp = int(pc.getTemp(hw))
    load = int(pc.getLoad(hw))

    rpm = controller.setRPM(temp)
    srpm = controller.smoothRPM(rpm)

    tram, _, uram = pc.getRAM(wmi)

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