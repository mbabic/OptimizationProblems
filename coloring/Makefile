# Knapsack solver Makefile.
# http://www.github.com/OptimizationProblems
# Marko Tomislav Babic mbabic@ualberta.ca

# Directories
BIN = bin
SRC = src

# Compiler and compiler options
CC = gcc
CFLAGS = -c
LDFLAGS = 

SOURCES = $(SRC)/main.c $(SRC)/graph.c $(SRC)/graph.h $(SRC)/utils.h
SOURCES += $(SRC)/coloring_solver.c $(SRC)/coloring_solver.h $(SRC)/pqueue.c
SOURCES += $(SRC)/pqueue.h
OBJS = $(BIN)/main.o $(BIN)/graph.o $(BIN)/coloring_solver.o $(BIN)/pqueue.o
EXE = coloring_solver

all: CFLAGS += -O3
all: $(EXE)

debug: CFLAGS += -g -DDEBUG -Wall -pg
debug: LDFLAGS += -pg
debug: $(EXE)

clean:
	rm $(BIN)/*.o $(BIN)/$(EXE)

$(EXE): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(BIN)/$@

$(OBJS): $(SOURCES)
	$(CC) $(CFLAGS) $(subst bin,src,$(subst .o,.c,$@)) -o $@


