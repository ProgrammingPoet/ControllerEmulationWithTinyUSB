#Client.py
import ctypes
import socket
import time

class XINPUT_GAMEPAD(ctypes.Structure):
    _fields_ = [
        ("wButtons", ctypes.c_ushort),
        ("bLeftTrigger", ctypes.c_ubyte),
        ("bRightTrigger", ctypes.c_ubyte),
        ("sThumbLX", ctypes.c_short),
        ("sThumbLY", ctypes.c_short),
        ("sThumbRX", ctypes.c_short),
        ("sThumbRY", ctypes.c_short)
    ]

class XINPUT_STATE(ctypes.Structure):
    _fields_ = [
        ("dwPacketNumber", ctypes.c_ulong),
        ("Gamepad", XINPUT_GAMEPAD)
    ]

try:
    XINPUT_DLL = ctypes.windll.xinput1_4
    print("xinput1_4.dll Loaded")
except:
    print("Unable To Load xinput1_4.dll")
    
XINPUT_DLL.XInputGetState.argtypes = [ctypes.c_ulong, ctypes.POINTER(XINPUT_STATE)]
XINPUT_DLL.XInputGetState.restype = ctypes.c_ulong
GAMEPAD_STATE = XINPUT_STATE()

UDP_SOCKET = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

while True:
    ERROR = XINPUT_DLL.XInputGetState(1, ctypes.byref(GAMEPAD_STATE))

    if ERROR == 0:
        PAYLOAD = bytes(GAMEPAD_STATE.Gamepad)
        UDP_SOCKET.sendto(PAYLOAD, ("192.168.4.1", 8080))
        time.sleep(0.001)
    else:
        print("Unable To Get Xinput Gamepad State From xinput1_4.dll, Error Code: ", ERROR)