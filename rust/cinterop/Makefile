all: hello-c libhello.so

libhello.so: Makefile
	gcc -fPIC -shared -o libhello.so hello.c

hello-c: hello-app.c libhello.so Makefile
	gcc -o hello-c hello-app.c -Wl,-rpath=. -L. -lhello

clean:
	rm hello-c libhello.so

.PHONY: clean