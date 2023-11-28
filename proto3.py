import RPi.GPIO as GPIO
import time

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)

delay = 0.00001
boardSize = 8

# output pins
#power pin 1
#GND pin
# assigned to BCM pin num, comment is regular pin num
LEDLatch = 17       # pin 11    'top' RCLK
LEDOut = 27         # pin 13    'top' OE
LEDShift = 22       # pin 15    'top' SRCLK
masterReset = 4     # pin 7      SRCLR
LEDData = 5         # pin 29    'top' SER
rowData = 6         # pin 31    'bottom' SER
rowLatch = 13       # pin 33    'bottom' RCLK
rowOut = 19         # pin 35    'bottom' OE
rowShift = 26       # pin 37    'bottom' SRCLK

# input pins
bitZeroPin = 18     # pin 12 COL1
bitOnePin = 23      # pin 16 COL2
bitTwoPin = 24      # pin 18
bitThreePin = 25    # pin 22
bitFourPin = 12     # pin 32
bitFivePin = 16     # pin 36
bitSixPin = 20      # pin 38
bitSevenPin = 21    # pin 40

# set up data input pins
inputPins = [bitZeroPin,bitOnePin,bitTwoPin,bitThreePin,bitFourPin,bitFivePin,bitSixPin,bitSevenPin]
for pinNum in inputPins:
    GPIO.setup(pinNum, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

# set up shift reg output pins
outputPins = [LEDLatch,LEDOut,LEDShift,masterReset,LEDData,rowData,rowLatch,rowOut,rowShift]
for pinNum in outputPins:
    GPIO.setup(pinNum, GPIO.OUT)

# initialize logic
def setupBoard():
    GPIO.output(LEDOut,GPIO.HIGH)
    GPIO.output(LEDShift,GPIO.LOW)
    GPIO.output(LEDLatch,GPIO.LOW)
    GPIO.output(LEDData,GPIO.LOW)

    GPIO.output(rowOut,GPIO.HIGH)
    GPIO.output(rowShift,GPIO.LOW)
    GPIO.output(rowLatch,GPIO.LOW)
    GPIO.output(rowData,GPIO.LOW)

    GPIO.output(masterReset,GPIO.LOW)
    time.sleep(1)
    GPIO.output(masterReset,GPIO.HIGH)
    return None

def sendData(data,serialPin,latchPin,shiftPin):
    if data==1:
        GPIO.output(serialPin,GPIO.HIGH)
        time.sleep(delay)
    else:
        GPIO.output(serialPin,GPIO.LOW)
        time.sleep(delay)
    GPIO.output(latchPin,GPIO.HIGH)
    time.sleep(delay)
    GPIO.output(shiftPin,GPIO.HIGH)
    time.sleep(delay)
    GPIO.output(latchPin,GPIO.LOW)
    time.sleep(delay)
    GPIO.output(shiftPin,GPIO.LOW)
    time.sleep(delay)

def decodeData():
    temp=-1
    for row in range(boardSize):
        sendData(0,rowData,rowLatch,rowShift)
        for col in range(boardSize):
            sendData(0,LEDData,LEDLatch,LEDShift)
            sendData(0,LEDData,LEDLatch,LEDShift)
    for row in range(boardSize):
        GPIO.output(rowOut,GPIO.HIGH)
        time.sleep(delay)
        if row == 0:
            sendData(1,rowData,rowLatch,rowShift)
            sendData(0,rowData,rowLatch,rowShift)
        else:
            sendData(0,rowData,rowLatch,rowShift)
        GPIO.output(rowOut,GPIO.LOW)
        for i in range(boardSize):
            if GPIO.input(inputPins[i]): 
                temp = row*boardSize+i
    GPIO.output(rowOut,GPIO.HIGH)
    return temp

def update(greenData,blueData):
    buttonIndex=-1
    for row in range(boardSize):
        sendData(1,rowData,rowLatch,rowShift)
    for row in range(boardSize):
        GPIO.output(LEDOut,GPIO.HIGH)
        GPIO.output(rowOut,GPIO.HIGH)
        time.sleep(delay)
        if row == 0:
            sendData(0,rowData,rowLatch,rowShift)
            sendData(1,rowData,rowLatch,rowShift)
        else:
            sendData(1,rowData,rowLatch,rowShift)
        for col in range(boardSize):
            sendData(greenData[col+row*boardSize],LEDData,LEDLatch,LEDShift)
            sendData(blueData[col+row*boardSize],LEDData,LEDLatch,LEDShift)
        print('row', row)
        GPIO.output(LEDOut,GPIO.LOW)
        GPIO.output(rowOut,GPIO.LOW)
        time.sleep(0.00001)
    GPIO.output(LEDOut,GPIO.HIGH)
    GPIO.output(rowOut,GPIO.HIGH)
    buttonIndex = decodeData()
    return buttonIndex
    
def revRow(data):
    tempData = [0] * (boardSize * boardSize)
    for row in range(boardSize):
        for col in range(boardSize):
            tempData[(row*boardSize)+col]=data[(row*boardSize)-col+boardSize-1]
    return tempData

# main loop
setupBoard()
greenData = [1]*64
blueData = [0]*(boardSize*boardSize)
GPIO.output(masterReset,GPIO.HIGH)

GPIO.output(LEDOut,GPIO.LOW)
GPIO.output(rowOut,GPIO.LOW)
time.sleep(1)
while True:
    inputVal=update(revRow(greenData),revRow(blueData))
    if inputVal !=-1:
        if greenData[inputVal]==0 and blueData[inputVal]==0:
            greenData[inputVal]=1
            blueData[inputVal]=0
        elif greenData[inputVal]==1 and blueData[inputVal]==0:
            greenData[inputVal]=0
            blueData[inputVal]=1
        elif greenData[inputVal]==0 and blueData[inputVal]==1:
            greenData[inputVal]=1
            blueData[inputVal]=1
        else:
            greenData[inputVal]=0
            blueData[inputVal]=0
        print(inputVal, 'button was pressed with green:', greenData[inputVal], 'and blue:',blueData[inputVal])
