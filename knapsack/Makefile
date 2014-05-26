# Knapsack solver Makefile.
# http://www.github.com/OptimizationProblems
# Marko Tomislav Babic mbabic@ualberta.ca

# Directories
BIN = bin
SRC = src

# Compiler and compiler options
CC = gcc
CFLAGS = -c

SOURCES = $(SRC)/main.c $(SRC)/solver.h $(SRC)/solver.c $(SRC)/item.c 
SOURCES += $(SRC)/item.h $(SRC)/utils.h $(SRC)/pqueue.c $(SRC)/pqueue.h
OBJS = $(BIN)/main.o $(BIN)/solver.o $(BIN)/pqueue.o $(BIN)/item.o
EXE = knapsack_solver

all: CFLAGS += -O3
all: $(EXE)

debug: CFLAGS += -g -DDEBUG -Wall
debug: $(EXE)

clean:
	rm $(BIN)/*.o $(BIN)/$(EXE)


$(EXE): $(OBJS)
	$(CC) $(OBJS) -o $(BIN)/$@

$(OBJS): $(SOURCES)
	$(CC) $(CFLAGS) $(subst bin,src,$(subst .o,.c,$@)) -o $@

test: $(SOURCES) test.c
	gcc -g -c src/pqueue.c -o pqueue.o
	gcc -g -c test.c -o test.o
	gcc test.o pqueue.o -o test

