SRC:=$(wildcard *.c)
test:$(SRC)
	gcc $^ -o $@ -lcrypt -lssl -lcrypto
clean:
	rm test

