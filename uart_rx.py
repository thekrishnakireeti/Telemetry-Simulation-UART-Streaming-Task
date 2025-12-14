import serial
import csv

ser = serial.Serial("/dev/pts/3", 9600, timeout=1)
csv_file = open("output.csv", "w", newline="")
writer = csv.writer(csv_file)
writer.writerow(["timestamp","ax","ay","az","gx","gy","gz","alt","temp"])

print("Listening on /dev/pts/3...")

def checksum_ok(line):
    try:
        data, chk = line.strip()[1:].split('*')
        calc = 0
        for c in data:
            calc ^= ord(c)
        return ("%02X" % calc) == chk
    except:
        return False

while True:
    line = ser.readline().decode().strip()
    if line and checksum_ok(line):
        data = line[1:].split('*')[0].split(',')[1:]
        writer.writerow(data)
        csv_file.flush()
        print("Received:", data)

