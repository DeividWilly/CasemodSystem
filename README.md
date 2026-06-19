
# FanControllerESP32 

### This project aims to control a fan using a Python script in conjunction with an ESP32 board.

> #### Disclaimer: This project is completely amateur; I am not a profissional programmer. 90% script by chatgpt <3 

Logic: Script to define the rotations per second (RPM) the fan should operate at;<br
The script will provide struct with five data points: Temperature, CPU Load, Total RAM, RAM Usage, and Target Fan RPM.

```   
    	{
    	"temp": 63,		# Temperature
    	"load": 8.0,	# CPU Load
    	"rpm": 40,		# fanRPM	
    	"uram": 11.6,	# RAM Usage
    	"tram": 19.7	# Total RAM

    	}
```
For fan control, only the target RPM would be necessary, but I also want to include an e-paper display showing some information on the machine's case.

```
microcontroller : ESP32
epaper display : WeaCT 3.7"
OS: Windows 11 (for LibreHardwareMonitor)
```
>>>>>>> 271f40f1e75e2ce82dd958e39cd9e5371d806d30

