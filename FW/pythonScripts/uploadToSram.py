import sys
import serial
import time
import argparse

# Script uploads (manually!) data to "flash" and sram of the cartridge
# Used for preliminary testing

parser = argparse.ArgumentParser(
                    prog = 'Cartridge uploader',
                    description = 'Uploads data to the cartridge, currently just SRAM',
                    epilog = 'Wroom')
                    
parser.add_argument('-p', '--port', required=True)
parser.add_argument('-d', '--data', required=True)  # Data file
parser.add_argument('-r', '--ram', required=False)  # SRAM file, if provided
parser.add_argument('-RB', '--reset-before', action='store_true')    # Set reset before uploading
parser.add_argument('-RA', '--reset-after', action='store_true')    # Unset reset after uploading


args = parser.parse_args()

file = open(args.data, 'rb')    # Read file as binary
data = file.read()              # Read everything at once, iterate later
file.close()

if (len(data) > 32768):
    print("Data file too large (%d vs max 32768)..." % (len(data)))
    sys.exit(-1)

sram = None
if (args.ram != None):
    file = open(args.ram, 'rb')
    sram = file.read()
    file.close()
    
    if (len(sram) > 8192):
        print("SRAM file too large (%d vs max 8192)..." % (len(sram)))
        sys.exit(-1)

    
ser = None
ser = serial.Serial(args.port) # No baud necessary
#ser.open()

if (args.reset_before == True):
    toSend = b"SET RST 1\n"
    ser.write(toSend)
    ser.readline()  # Should be OK, but whatever

address = 0
chunkSize = 512
while (address < len(data)):
    print(address)
    toSend = b"WRITEB %X" %(address)
    if ( (len(data) - address) >= chunkSize):
        for i in range(0, chunkSize):
            toSend = toSend + b" %X" % (data[address + i])
        address = address + chunkSize
        
    else:
        toSend = b"WRITEB %X %X" % (address, data[address])
        address = address + 1
    
    toSend = toSend + b"\n"
    print(toSend)
    ser.write(toSend)
    temp = ser.readline()  # Should be OK, but whatever
    #print(temp)

'''
if (sram != None):
    address = 0xA000    # Here lies cartridge SRAM
    for b in sram:
        toSend = b"WRITEB %X %X\n" % (b, address)
        address = address + 1
        ser.write(toSend)
        ser.readline()  # Should be OK, but whatever
'''

if (args.reset_after == True):
    toSend = b"SET RST 0\n"
    ser.write(toSend)
    ser.readline()  # Should be OK, but whatever



print("All done")
    
ser.close()
