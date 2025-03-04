CC = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

# the build target executable:
TARGET1 = test
TARGET2 = aes
TARGET3 = sha256

LIBS = -L. -lpbc -lgmp

all: $(TARGET1).o $(TARGET2).o $(TARGET3).o
	$(CC) $(CFLAGS) $(TARGET1).o $(TARGET2).o $(TARGET3).o -o $(TARGET1)
	
$(TARGET1).o: $(TARGET1).cpp
	$(CC) -c $(TARGET1).cpp $(LIBS)
	
$(TARGET2).o: ./src/$(TARGET2).cpp ./src/$(TARGET2).h
	$(CC) -c ./src/$(TARGET2).cpp
	
$(TARGET3).o: ./src/$(TARGET3).cpp ./src/$(TARGET3).h
	$(CC) -c ./src/$(TARGET3).cpp

clean:
	$(RM) *.o $(TARGET1)
