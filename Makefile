all:
	mkdir -p bin
	gcc -D _GNU_SOURCE -lX11 -lpng -Wall -O3 -o bin/wms src/wms.c

install:
	sudo cp bin/wms /usr/bin/wms
