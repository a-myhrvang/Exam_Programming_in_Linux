# Makefile template

INCLDIR = include
CC = gcc
CFLAGS = -O2
CFLAGS += -I$(INCLDIR) -pthread

OBJDIR = obj

_DEPS = task4_threads.h djb2.h tea.h pgdbglog.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = task4_threads.o djb2.o tea.o pgdbglog.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

task4_threads: $(OBJS)
	gcc -o $@ $^ $(CFLAGS) -pthread -lrt

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~
