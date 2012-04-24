
CUTEST_TESTBIN := CuTest_test


CFLAGS += -Wall -pedantic -fprofile-arcs -ftest-coverage -g 

all: $(CUTEST_TESTBIN)

run_test: $(CUTEST_TESTBIN)
	./$(CUTEST_TESTBIN)

lcov:
	lcov -c --directory . -o lcov.info

coverage: run_test lcov
	genhtml -o coverage lcov.info

$(CUTEST_TESTBIN): CuTest.o CuTestTest.o AllTests.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

CuTest.o: CuTest.c CuTest.h

clean:
	rm -rf coverage lcov.info *.o *.gcda *.gcno *.gcov $(CUTEST_TESTBIN)
