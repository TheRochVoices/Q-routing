from datetime import datetime

rec = "12,1212"

def handleData(rec):
	sep = rec.find(',')
	length = len(rec)	
	print("Device ID: ",rec[:sep])
	print("Packet: ",rec[sep+1:length])
	did = rec[:sep]
	pkt = rec[sep+1:length]
	name = "log" + did + ".csv"
	print("Filename: ",name)
	dt = str(datetime.now())
	print("Current time: ",dt)
	date = dt[:dt.find(' ')]
	print("Date: ", date)
	time = dt[dt.find(' ') +1: len(dt)]
	print(time)
	writeData(name, pkt, date, time)

def writeData(name, pkt, date, time):
	f = open(name, 'a')
	toWrite = date + ", " + time + ", " + pkt + "\n"
	print("Writing ", toWrite, " to the log file")
	f.write(toWrite)
	print("Write Done")
	f.close()
	
	

for i in range(0,20):
		
	handleData(rec + str(i))
