from SerialWrapper import SerialWrapper
from AudioProcess import AudioProcess
from LedHelpers import *
import time
import os

clear = lambda: os.system('cls')

ctrl = AudioProcess()
print(ctrl.sessions)

srl = SerialWrapper()
print(srl.ports)
srl.assign_device("COM3")
print(srl.device)

vol = ctrl.get_volume_level("Spotify.exe")
mute = 0

start_srl = False

while True:
    try:
        data_in = srl.read_data().decode("utf-8")
        if data_in == "BEGIN\r\n":
            start_srl = True
            print(srl.read_data().decode("utf-8"))
        elif data_in == "END\r\n":
            srl.send_data("ENDED\r".encode())
            start_srl = False
        elif data_in == "APPS\r\n":
            srl.send_data("apps\r".encode())
            srl.send_data("Spotify,App2,App3,App4,\r".encode())
    except Exception as err:
        print("An exception occurred", err)
        # break
    if start_srl:
        clear()
        meter = [0] * 10
        peak = ctrl.get_speaker_volume() * 10
        val1 = int(peak // 1)
        val2 = round(peak % 1, 1)
        for i in range(0, val1):
            meter[i] = 1
        if val1 < 10:
            meter[val1] = val2

        print(brightness(meter))

        try:
            if data_in.split(",")[0] == "START":
                leds = data_in.split(",")
                print(leds)
                try:
                    if vol != int(leds[1])/100:
                        vol = int(leds[1])/100
                        ctrl.set_volume("Spotify.exe", vol)
                    if mute != int(leds[5]):
                        mute = int(leds[5])
                        ctrl.mute("Spotify.exe", mute)
                except:
                    print("An exception occurred", leds[0:len(leds)])
            if data_in.split(",")[-1] == "STOP\r\n":
                srl.send_data("leds\r".encode())
                srl.send_data(brightness(meter))
        except Exception as err:
            print("An exception occurred", err)