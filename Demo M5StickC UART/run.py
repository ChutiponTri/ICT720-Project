import pandas as pd
import serial
import json

data = []
ser = serial.Serial("COM3", 115200)
flag = True

try:
    while flag:
        val = ser.readline()
        val = json.loads(val)
        data.append(val["az"])
        print(val)
except:
    flag = False
    print(len(data))

    data_to_save = {"data": data}
    df = pd.DataFrame(data_to_save)
    df.to_excel("data.xlsx", index=False)