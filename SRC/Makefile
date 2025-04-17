# Définition des variables
CC = gcc
IDIR = INCLUDE
ODIR = OBJ
CFLAGS = -Wall -O0 -g
TARGET = MOE-P

_DEPS = MOW-P.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = MOW-P.o ReadParameters.o ReadProblem.o eprintf.o printff.o ReadLine.o InitMu.o \
	   fscanint.o Distance.o Forbidden.o ReadInitTours.o InitQuality.o BitFlip.o InitToursCost.o \
	   Allocate.o DeepCopy.o GetCurrentQuality.o WriteSolution.o GetDimension.o GetTime.o \
	   Init.o WriteInitTours.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY:
	all clean

all:
	$(MAKE) MOW-P

MOW-P: $(OBJ) $(DEPS)
	$(CC) -o ../MOW-P $(OBJ) $(CFLAGS) -lm

clean:
	/bin/rm -f $(ODIR)/*.o ../MOW-P *~ ._* $(IDIR)/*~ $(IDIR)/._*
