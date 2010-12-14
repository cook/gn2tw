CPPFLAGS+=-I/usr/include/libxml2
CFLAGS+=-O2
LDLIBS+=-lxml2 -lsakcore
LDFLAGS+=-Llib

gn2tw:tw.o

clean:
	@if [[ -a gn2tw ]]; then rm -f gn2tw; fi
	@rm *.o