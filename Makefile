all:
	mkdir -p bin
	gcc -o bin/alloc.exe allocator.c lib/gc_alloc.c

debug:
	mkdir -p bin
	gcc -o bin/alloc.exe -D GC_DEBUG allocator.c lib/gc_alloc.c

run:
	./bin/alloc.exe