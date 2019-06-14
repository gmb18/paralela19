CFLAGS  = -std=c99 \
	  -pipe \
          -W -Wall -Wextra \
	  -Wbad-function-cast \
	  -Wcast-align \
	  -Wcast-qual \
	  -Wconversion \
	  -Wfloat-equal \
	  -Wformat-y2k \
	  -Winit-self \
	  -Winline \
	  -Winvalid-pch \
	  -Wmissing-declarations \
	  -Wmissing-field-initializers \
	  -Wmissing-format-attribute \
	  -Wmissing-include-dirs \
	  -Wmissing-noreturn \
	  -Wmissing-prototypes \
	  -Wnested-externs \
	  -Wnormalized=nfc \
	  -Wold-style-definition \
	  -Woverlength-strings \
	  -Wpacked \
	  -Wpadded \
	  -Wpointer-arith \
	  -Wredundant-decls \
	  -Wshadow \
	  -Wsign-compare \
	  -Wstack-protector \
	  -Wstrict-prototypes \
	  -Wundef \
	  -Wvolatile-register-var \
	  -Wwrite-strings

#------------------------------------------------------------------------------
.PHONY : all clean

#------------------------------------------------------------------------------
all: misto quicksort mergesort parallel1

misto : misto.o sorting.o
	$(CC) $(CFLAGS) -o $@ $^

quicksort : quicksort.o sorting.o
	$(CC) $(CFLAGS) -o $@ $^

mergesort : mergesort.o sorting.o
	$(CC) $(CFLAGS) -o $@ $^

sorting.o: sorting.c sorting.h
	$(CC) -c $(CFLAGS) -O3 sorting.c -o sorting.o

stdquicksort : stdquicksort.o
	$(CC) $(CFLAGS) -o $@ $^

parallel1 : parallel1.c sorting.o
	$(CC) $(CFLAGS) -o $@ $^ -fopenmp -O3

mpi : mpi.c
	mpicc $(CFLAGS) mpi.c -o mpi -O3

#------------------------------------------------------------------------------
clean :
	$(RM) *.o misto quicksort mergesort stdquicksort parallel1
