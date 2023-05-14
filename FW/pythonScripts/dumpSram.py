import sys
import serial
import time
import argparse

parser = argparse.ArgumentParser(
                    prog = 'Cartridge reader',
                    description = 'Reads data from the cartridge',
                    epilog = 'Wroom')
                    
parser.add_argument('-p', '--port', required=True)
parser.add_argument('-d', '--data', required=True)  # Data file to save
parser.add_argument('-r', '--ram', required=False)  # SRAM file, if provided
parser.add_argument('-RB', '--reset-before', action='store_true')    # Set reset before uploading
parser.add_argument('-RA', '--reset-after', action='store_true')    # Unset reset after uploading


args = parser.parse_args()

ser = serial.Serial(args.port)

file = open(args.data, 'wb')    # Write file as binary

address = 0
readSize = 32
while (address <= (32768-readSize)):
    toSend = b"READB %x %x\n" % (address, readSize)
    ser.write(toSend)
    readout = ser.readline()
    print(address)
    print(readout)
    tokens = readout.split(b' ')
    tokens.pop(0)   # Remove OK
    for b in tokens:
        val = bytes([int(b, 16)])
        file.write(val)
    address = address + readSize
        

print("All done")
    
ser.close()
