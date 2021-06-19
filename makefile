default: heap_test

heap.o: heap.c heap.h
	clang -g -c heap.c -o heap.o

heap_test.o: heap_test.c heap.c heap.h
	clang -c heap_test.c -o heap_test.o

heap_test: heap.o heap_test.o
	clang heap.o heap_test.o -o heap_test

day15.o: day15.c heap.h
	clang -c day15.c -o day15.o

day15: day15.o heap.o
	clang -O3 day15.o heap.o -o day15

clean:
	-rm -f *.o
