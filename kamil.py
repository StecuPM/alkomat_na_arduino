
import serial
import csv
import re
import pandas as pd
 
portPath = "/dev/ttyACM0"       # Must match value shown on Arduino IDE
baud = 9600                # Must match Arduino baud rate
timeout = 5                       # Seconds
filename = "data.csv"
max_num_readings = 16000
num_signals = 1
 
def create_serial_obj(portPath, baud_rate, tout):
    """
    Given the port path, baud rate, and timeout value, creates
    and returns a pyserial object.
    """
    return serial.Serial(portPath, baud_rate, timeout = tout)
    
def read_serial_data(serial):
    """
    Given a pyserial object (serial). Outputs a list of lines read in
    from the serial port
    """
    serial.flushInput()
    
    serial_data = []
    readings_left = True
    timeout_reached = False
    
    while readings_left and not timeout_reached:
        serial_line = serial.readline()
        if serial_line == '':
            timeout_reached = True
        else:
            serial_data.append(serial_line)
            if len(serial_data) == max_num_readings:
                readings_left = False
        
    return serial_data
 
def is_number(string):
    """
    Given a string returns True if the string represents a number.
    Returns False otherwise.
    """
    try:
        float(string)
        return True
    except ValueError:
        return False
        
def clean_serial_data(data):
    """
    Given a list of serial lines (data). Removes all characters.
    Returns the cleaned list of lists of digits.
    Given something like: ['0.5000,33\r\n', '1.0000,283\r\n']
    Returns: [[0.5,33.0], [1.0,283.0]]
    """
    clean_data = []
    
    for line in data:
        line_data = re.findall("\d*\.\d*|\d*",line) # Find all digits
        line_data = [float(element) for element in line_data if is_number(element)] # Convert strings to float
        if len(line_data) >= 2:
            clean_data.append(line_data)
 
    return clean_data           
 
def save_to_csv(data, filename):
    """
    Saves a list of lists (data) to filename
    """
    with open(filename, 'wb') as csvfile:
        csvwrite = csv.writer(csvfile)
        csvwrite.writerows(data)

print( "Creating serial object...")
serial_obj = create_serial_obj(portPath, baud, timeout)
 
print ("Reading serial data...")
serial_data = read_serial_data(serial_obj)
print (len(serial_data))
 
print ("Cleaning data...")
clean_data =  clean_serial_data(serial_data)
 
print ("Saving to csv...")
save_to_csv(clean_data, filename)
 
