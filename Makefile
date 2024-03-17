
# Makefile for Practicum 1

CC = gcc

# Target and source
TARGET = prog
SRC1 = MessageStructure.c
SRC2 = CS5600.Practicum1.c
DEPENDENCY = practicum

all: $(DEPENDENCY)

$(DEPENDENCY):
	$(CC) -o $(TARGET) $(SRC1) $(SRC2) 

run:
	./$(TARGET) 5 LRU
	
clean:
	rm -f $(TARGET)


