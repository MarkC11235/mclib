all: run_test

build_test: mclib_test.c mclib.h
	gcc -o mclib_test mclib_test.c

run_test: build_test
	./mclib_test

