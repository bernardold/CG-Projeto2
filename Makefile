CC = g++
CFLAGS = -std=c++11
PROG = projeto2
SRCS = projeto2.cpp
LIBS = -lGLU -lGL -lglut -lSOIL -lm -pthread

all: $(PROG)

$(PROG): $(SRCS)
	$(CC) $(SRCS) $(LIBS) $(CFLAGS) -o $(PROG)

clean:
	rm -f $(PROG)

run:
	./$(PROG)