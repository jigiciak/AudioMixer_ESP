import serial
import serial.tools.list_ports


class SerialWrapper:
    def __init__(self):
        self.device = None
        self.baudrate = 115200
        self.ports = {}
        self.list_ports()

    def send_data(self, data):
        self.device.write(bytearray(data))

    # def send_start(self):
    #     self.device.write(bytes(b'START'))

    def read_data(self):
        return self.device.read()

    def list_ports(self):
        for i, port in enumerate(serial.tools.list_ports.comports()):
            self.ports[port.device] = port.description

    def assign_device(self, device):
        self.device = serial.Serial(device, self.baudrate)