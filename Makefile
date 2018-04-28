
IDIR =./
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=./
LDIR =./

LIBS=

_DEPS = readCSVFile.h cmdopt.h util.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = csvCmd.o util.o readCSVFile.o cmdopt.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

csvcmd: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 