all:
	mkdir -p bin
	gcc -o bin/alloc.exe allocator.c

run:
	./bin/alloc.exe