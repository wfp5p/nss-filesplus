CC = gcc
OBJECTS=passwd.o common.o group.o shadow.o
SHARED_OBJECT = libnss_filesplus.so.2
#CPPFLAGS = -DDEBUG
CFLAGS = -g -O2 -fPIC -Wall -Wextra
LIBS = -lc
LDLIBFLAGS = -shared -Wl,-soname,$(SHARED_OBJECT)
LDFLAGS = -Wl,-z,defs

all: $(SHARED_OBJECT)

$(SHARED_OBJECT): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDLIBFLAGS) $(LDFLAGS) -o $(SHARED_OBJECT) $(OBJECTS) $(LIBS)

%.o: %.c nss-filesplus.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -c -o $@ $<

clean:
	rm -f $(OBJECTS)
	rm -f $(SHARED_OBJECT)

distclean: clean

.PHONY: all
