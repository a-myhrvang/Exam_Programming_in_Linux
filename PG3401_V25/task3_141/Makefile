# Makefile template

INCLDIR = include
CC = gcc
CFLAGS = -O2
CFLAGS += -I$(INCLDIR) -include stdio.h

OBJDIR = obj

_DEPS = pgdbglog.h flight.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = main.o pgdbglog.o flight.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJS)
	gcc -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~
