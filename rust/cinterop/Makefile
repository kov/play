all: hello-c hello-rust libhello.so

libhello.so: hello.c hello.h Makefile
	gcc -fPIC -shared -o libhello.so hello.c

hello-c: hello-app.c libhello.so Makefile
	gcc -o hello-c hello-app.c -Wl,-rpath=. -L. -lhello

# FIXME: hardcoded paths because I couldn't figure out how to tell
# rustc that it should give -L. to gcc when linking
hello-rust: hello.rs libhello.so Makefile
	rustc -c hello.rs
	gcc -L . -L/home/kov/.local/lib/rustc/x86_64-unknown-linux-gnu/lib -m64 -o hello-rust hello-rust.o -L/home/kov/.local/lib/rustc/x86_64-unknown-linux-gnu/lib -lcore-14bd852465126fe7-0.1 -lhello -lm -lrustrt -lrt -ldl -lmorestack -Wl,-rpath,/home/kov/.local/lib/rustc/x86_64-unknown-linux-gnu/lib -Wl,-rpath,/home/kov/.local/lib/rustc/x86_64-unknown-linux-gnu/lib

clean:
	rm hello-rust hello-rust.o hello-c libhello.so

.PHONY: clean