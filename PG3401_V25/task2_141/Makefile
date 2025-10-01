# Makefile template

INCLDIR = include
CC = gcc
CFLAGS = -O2
CFLAGS += -I$(INCLDIR) -include stdio.h

OBJDIR = obj

_DEPS = task2_count.h task2_sum.h task2_hash.h pgdbglog.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = task2_count.o task2_sum.o task2_hash.o main.o pgdbglog.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJS)
	gcc -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~
