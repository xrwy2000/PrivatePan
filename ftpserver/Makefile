SRC:=$(wildcard *.c)
thread_pool_server:$(SRC)
	gcc $^ -o $@ -lpthread -I /usr/include/mysql/ -lmysqlclient -lcrypt
clean:
	rm thread_pool_server

