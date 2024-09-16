# Variables
CC ?= clang
CFLAGS ?= -w
LDFLAGS = -lssl -lcrypto
TARGET = PasswordHasher
SRC = PasswordHasher.c

# Default target
all: $(TARGET)

# Compile target
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

# Clean target
clean:
	rm -f $(TARGET)

.PHONY: all clean