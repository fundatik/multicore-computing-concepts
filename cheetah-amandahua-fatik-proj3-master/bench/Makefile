include ../config.mk
# include ../cilkplus-config.mk
SRCS = $(wildcard *.c)
HDRS = $(wildcard *.h)
OBJS = $(patsubst %.c,%.o,$(SRCS))

TESTS          = cholesky fib fft mm_dac cilksort heat lu nqueens strassen
OPTIONS = -O3 -g -fcilkplus $(RTS_OPT) -fno-exceptions # -Werror -Wall 
# dynamic linking
RTS_DLIBS += -L$(RTS_DIR) -Wl,-rpath -Wl,$(RTS_DIR) $(RTS_LIB_FLAG)  
LD_FLAGS = $(RTS_DLIBS)
# static linking
# RTS_SLIBS = $(RTS_DIR)/$(RTS_LIB).a
# LD_FLAGS = $(RTS_SLIBS)
TIMING_COUNT := 5

all: $(TESTS)

$(TESTS): %: %.o getoptions.o ktiming.o
	$(LINK_CC) $^ -o $@ $(LD_FLAGS) -lrt -lpthread -lm -ldl

%.o: %.c
	$(CC) -c $(OPTIONS) -DTIMING_COUNT=$(TIMING_COUNT) -o $@ $<

check:
	./fib 42 --nproc 16
	./cholesky --nproc 16 -n 4000 -z 40000 -c
	./fft --nproc 16 -n 67108864
	./heat --nproc 16 -nx 2048 -ny 2048 -nt 500
	./lu --nproc 16 -n 4096 -c
	./mm_dac --nproc 16 -n 2048 -c
	./nqueens --nproc 16 14
	./strassen --nproc 16 -n 4096 -c

clean:
	rm -f *.o *~ $(TESTS) core.*

reallyclean:
	rm -f *.o *~ $(TESTS) core.* *-timing.txt
