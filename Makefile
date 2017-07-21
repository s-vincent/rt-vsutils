CFLAGS = -std=c11 -Wall -Wextra -Werror -Wstrict-prototypes -Wredundant-decls -Wshadow -pedantic -pedantic-errors -fno-strict-aliasing -D_XOPEN_SOURCE=700 -O2 -I./include/rt-vsutils
LDFLAGS = -lpthread -lrt 
SOURCES = src/rtutils.c
OBJ = $(SOURCES:.c=.o)
TESTS = test_memlock test_affinity test_priority test_rt_priority test_cpufreq test_rt_watchdog test_periodic_task

all: $(OBJ)
	
tests: $(TESTS)

.c.o:
	$(CC) -g -c $(CFLAGS) $< -o $@

test_memlock: $(OBJ) tests/test_memlock.o
	$(CC) -o $@ $? $(LDFLAGS)

test_affinity: $(OBJ) tests/test_affinity.o
	$(CC) -o $@ $? $(LDFLAGS)

test_priority: $(OBJ) tests/test_priority.o
	$(CC) -o $@ $? $(LDFLAGS)

test_rt_priority: $(OBJ) tests/test_rt_priority.o
	$(CC) -o $@ $? $(LDFLAGS)

test_cpufreq: $(OBJ) tests/test_cpufreq.o
	$(CC) -o $@ $? $(LDFLAGS)

test_rt_watchdog: $(OBJ) tests/test_rt_watchdog.o
	$(CC) -o $@ $? $(LDFLAGS)

test_periodic_task: $(OBJ) tests/test_periodic_task.o
	$(CC) -o $@ $? $(LDFLAGS)

doc:
	rm -rf doc/html
	doxygen doc/Doxyfile

clean:
	echo rm $(OBJ) $(TESTS)
	rm -f src/*.o tests/*.o $(TESTS)
	rm -rf doc/html

.PHONY: doc

