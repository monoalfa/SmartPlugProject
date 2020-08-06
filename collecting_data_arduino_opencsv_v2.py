# -*- coding: utf-8 -*-
"""
Created on Wed Nov 13 13:54:24 2019

@author: Ingrid - CITCEA
"""
# importing libraries 
import serial
import datetime as dt 
import re
import csv 

# creating communications object using Serial 
arduino = serial.Serial('COM15', 115200)
print("Starting!")

# try-except-finally loop for data acquisition
try:
    while True:
          # Check if there is new info from the Arduino and read it
          data_bytes = arduino.readline() 
          # Decoding the message into UTF-8         
          data = data_bytes.decode("utf-8")
          
          # if data has been read, print and save it
          if data:
              print(data)
              # strip data string into 3 different values for each reading   
              l1,l2 = re.findall(pattern=r"[-+]?\d*\.\d+|\d+", string=data)
              # store data and readings into a list
              row = [dt.datetime.now(), float(l1), float(l2)]
          # save reading row into the csv file    
              with open('data.csv', 'a', newline='') as csvFile: 
                  writer = csv.writer(csvFile)
                  writer.writerow(row)
                  csvFile.close()
# handling KeyboardInterrupt by the end-user (CTRL+C)
except KeyboardInterrupt:
    # closing communications port
    arduino.close() 
    print('Communications closed')