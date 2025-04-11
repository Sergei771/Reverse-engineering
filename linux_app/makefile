CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic
LDFLAGS = 
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin

# Mode debug optionnel
ifeq ($(DEBUG),1)
    CFLAGS += -g -O0
else
    CFLAGS += -O2
endif

SRCS = main.c serial_handler.c command_validator.c ui_handler.c special_commands.c
OBJS = $(SRCS:.c=.o)
TARGET = stm32_console

.PHONY: all clean distclean install check_deps

all: check_deps $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c $(wildcard *.h)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

distclean: clean
	rm -f *~ *.bak

install: $(TARGET)
	install -d $(DESTDIR)$(BINDIR)
	install -m 755 $(TARGET) $(DESTDIR)$(BINDIR)

check_deps:
	@echo "Checking dependencies..."
	@which $(CC) > /dev/null || (echo "Error: $(CC) not found" && exit 1)
