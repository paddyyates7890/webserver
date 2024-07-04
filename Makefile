CFLAGS=-g -O2 -Wall -Wextra -Isrc -rdynamic

SOURCES=$(wildcard src/**/**/*.c src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o, $(SOURCES))

TARGET=build/webserver.a
SO_TARGET=$(patsubst %.a, %, $(TARGET))

all: $(TARGET) $(SO_TARGET)

$(TARGET): CFLAGS += -fPIC
$(TARGET): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@

$(SO_TARGET): $(TARGET) $(OBJECTS)
	gcc $(CFLAGS) -o $@ $(OBJECTS)


build:
	@mkdir -p build
	@cp index.html build/index.html

clean:
	rm -rf build $(objects)

BADFUNCS='[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'

check:
	@echo Files with potentially dangerous functions.
	@egrep $(BADFUNCS) $(SOURCES) || true


