set windows-shell := ["powershell.exe", "-c"]

build-allocators:
	mkdir -Force build
	clang -c -o build/allocators.o allocators.c
	clang -shared -o build/allocators.dll build/allocators.o
	
test-allocators:
	mkdir -Force build
	clang -Wall -o build/test_allocators.exe allocators_test.c allocators.c
	.\test_allocators.exe