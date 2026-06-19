import pathlib
import zipfile
import sys
import os
import shutil
import time
import xml.etree.ElementTree as ET
from urllib.request import urlretrieve

a = []

version_checksum = "086d9f1b5a99e643edc2cfaaac16051685b551e4c5ac0b32a57c58c0e529c001"
folder_contain = ['Aga.Controls.dll', 'Aga.Controls.pdb', 'BlackSharp.Core.dll', 'de', 'DiskInfoToolkit.dll', 'es', 'fr', 'HidSharp.dll', 'it', 'ja', 'LibreHardwareMonitor.config', 'LibreHardwareMonitor.exe', 'LibreHardwareMonitor.exe.config', 'LibreHardwareMonitorLib.dll', 'LibreHardwareMonitorLib.pdb', 'LibreHardwareMonitorLib.xml', 'Microsoft.Bcl.AsyncInterfaces.dll', 'Microsoft.Bcl.HashCode.dll', 'Microsoft.Win32.TaskScheduler.dll', 'OxyPlot.dll', 'OxyPlot.WindowsForms.dll', 'pl', 'RAMSPDToolkit-NDD.dll', 'README.md', 'ru', 'sv', 'System.Buffers.dll', 'System.CodeDom.dll', 'System.Collections.Immutable.dll', 'System.Formats.Nrbf.dll', 'System.IO.Pipelines.dll', 'System.Memory.dll', 'System.Numerics.Vectors.dll', 'System.Reflection.Metadata.dll', 'System.Resources.Extensions.dll', 'System.Runtime.CompilerServices.Unsafe.dll', 'System.Security.AccessControl.dll', 'System.Security.Principal.Windows.dll', 'System.Text.Encodings.Web.dll', 'System.Text.Json.dll', 'System.Threading.AccessControl.dll', 'System.Threading.Tasks.Extensions.dll', 'tr', 'zh-CN', 'zh-Hant']
version_app = "v0.9.6"
package_link = f"https://github.com/LibreHardwareMonitor/LibreHardwareMonitor/releases/download/{version_app}/LibreHardwareMonitor.zip"
fileZipName = "LibreHardwareMonitor.zip"
folderName = "LibreHardwareMonitor"
port = "8085"

def checkFile(zipName, folderName):
    zipPath = pathlib.Path(zipName)
    folderPath = pathlib.Path(folderName)
    
    zipExists = zipPath.exists()
    folderExists = folderPath.exists()
    
    return zipExists, folderExists

def downloadFile(url, fileName):
    print(f"Downloading LibreHardwareMonitor {version_app} from GitHub...")
    urlretrieve(url, fileName)
    print("Done!")
    
def extractFile(file):
    print("Unpacking...")
    try:
        with zipfile.ZipFile(file, 'r') as zip_ref:
            zip_ref.extractall("LibreHardwareMonitor")
    except PermissionError:
        f = pathlib.Path(file)
        f.unlink()
        print("File running. Please close the LibreHardwareMonitor process and try again.")
        sys.exit(1)
        
def configXML(configPath, port):
    tree = ET.parse(configPath)
    root = tree.getroot()

    app_settings = root.find(".//appSettings")
    if app_settings is None:
        print("appSettings not found!")
        return

    run_web_exists = False
    listener_port_exists = False

    for elem in app_settings.findall("add"):
        key = elem.get("key")
        
        if key == "runWebServerMenuItem":
            elem.set("value", "true")
            run_web_exists = True
            print("WebServer activated.")
            
        elif key == "listenerPort":
            elem.set("value", str(port))
            listener_port_exists = True
            print(f"WebServer port set to {port}")

    if not run_web_exists:
        ET.SubElement(app_settings, "add", key="runWebServerMenuItem", value="true")
        print("WebServer rule created and activated.")

    if not listener_port_exists:
        ET.SubElement(app_settings, "add", key="listenerPort", value=str(port))
        print(f"WebServer port rule created and set to {port}")
            
    tree.write(configPath, encoding="utf-8", xml_declaration=True)
    print(f"File saved in {configPath}")
    print("Manually restart the LibreHardwareMonitor process if it is already running to ensure the configuration is correct.")
            
check = checkFile(fileZipName, folderName)

if check[0] == True:
    
    print("Zip file has exists.")
    
    if check[1] == True:
        f = pathlib.Path(folderName)
        fz = pathlib.Path(fileZipName)
        for item in f.iterdir():
            a.append(item.name)
            
        if a == folder_contain:
            print("Files matches")
        else:
            print("Files don't matches, removing all files...")
            try:
                fz.unlink()
                shutil.rmtree(f)
            except FileNotFoundError:
                pass
            
    else:
            print("But folder dont exists")
else:
    print("Downloading...")
    downloadFile(package_link, fileZipName)
    extractFile(fileZipName)
    print("Please manually open and run the LibreHardwareMonitor.exe file, then close it immediately after opening to generate the XML file from your computer.")
    while True:
        confirm = input("Please confirm if you have already completed this.[Y/N]: ").upper().strip()
        if confirm == "Y":
            configXML("LibreHardwareMonitor/LibreHardwareMonitor.config", port)
            break
        elif confirm == "N":
            print("Please, re-run the script")
            break
        else:
            print("Invalid input, use Y or N")
print("Press ENTER to exit. ")