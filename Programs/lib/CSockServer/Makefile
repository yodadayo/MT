CC = g++ -Wall -g
TARGET = test_server1
OBJS = CSockServer.o test_server1.o
LIBS = -L/usr/lib -L/usr/local/lib
INDS = -I/usr/include -I/usr/local/include
OPTS = -lm
#for making object files
.cpp: $(CC) -c $< $(INDS)

$(TARGET) : $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS) $(INDS) $(OPTS)
clean:
	rm -f $(TARGET) $(OBJS) *~ core *.gch

test_server1.o : test_server1.cpp
CSockServer.o : CSockServer.cpp CSockServer.h
