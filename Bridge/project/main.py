import serialpy
import requests
import serial.tools.list_ports
from serial.rfc2217 import Serial


def main ():
    # show port available and print it
    print("list of available ports: ")
    ports = serial.tools.list_ports.comports()

    for p in ports:
        print(("Arduino Uno " + "(" + p.name + ")"))
        if (p.description == ("Arduino Uno " + "(" + p.name + ")")):
            ser = serial.Serial(p.name, 9600)
            break

    while True:
        val = ser.read(1)

        strval =  "{temperature:%d}" % int.from_bytes(val)
        print(strval)
        #print("{temperature:" + str(int(val.hex()) + "}"))
        r = requests.post("http://demo.thingsboard.io/api/v1/o0SzxyiH8fZWK1uQrRh2/telemetry", data=strval)


#entry point
if __name__ == '__main__':
    main()


