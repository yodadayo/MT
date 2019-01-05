all: copy main

HOST1 = lab@192.168.100.23:
DEST = /Users/lab/Documents/MT/Programs/
TASK = SAMPLE 

copy:
		rsync -rvpu4 --exclude=".git",".DS_Store" $(HOST1)$(DEST)  /home/pi/MT

main: ./moter_test.cpp
		g++ -o ./M_TEST ./moter_test.cpp ./lib/SlaveControlCommand.cpp ./lib/SPIM_Ras.cpp ./libdxl/DynamixelV2.cpp -l wiringPi -lrt -lbcm2835
