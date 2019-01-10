all: copy main

HOST1 = lab@192.168.100.202:
DEST = /Users/lab/Documents/MT/Programs/
TASK = SAMPLE 

copy:
		rsync -rvpu4 --exclude=".git" $(HOST1)$(DEST)  /home/pi/MT

main: ./MotionProgram.cpp
		g++ -o ./MOTION ./MotionProgram.cpp ./lib/SlaveControlCommand.cpp ./lib/SPIM_Ras.cpp ./libdxl/DynamixelV2.cpp -l wiringPi -lrt -lbcm2835
