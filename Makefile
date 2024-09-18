# Variables
CC ?= clang
CFLAGS ?= 
LDFLAGS = -lssl -lcrypto
SRC = PasswordHasher.c

# Default target
all: sha256depc

sha256depc: CFLAGS += -w
sha256depc: TARGET = sha256depcPasswdHash
sha256depc: $(SRC)
	$(CC) $(CFLAGS) -Dsha256depc -o $(TARGET) $(SRC) $(LDFLAGS)

EVP: TARGET = EVP_PasswdHash
EVP: $(SRC)
	$(CC) $(CFLAGS) -DEVP -o $(TARGET) $(SRC) $(LDFLAGS)

custom: TARGET = customPasswdHash
custom: $(SRC)
	$(CC) $(CFLAGS) -Dcustom -o $(TARGET) $(SRC)

# Clean target
clean:
	rm -f sha256depcPasswdHash EVP_PasswdHash customPasswdHash

.PHONY: clean