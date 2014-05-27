# Knapsack solver Makefile.
# http://www.github.com/OptimizationProblems
# Marko Tomislav Babic mbabic@ualberta.ca

# Directories
BIN = bin
SRC = src

# Compiler and compiler options
CC = gcc
CFLAGS = -c

SOURCES = $(SRC)/main.c $(SRC)/graph.c $(SRC)/graph.h $(SRC)/utils.h
SOURCES += $(SRC)/coloring_solver.c $(SRC)/coloring_solver.h
OBJS = $(BIN)/main.o $(BIN)/graph.o $(BIN)/coloring_solver.o 
EXE = coloring_solver

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


