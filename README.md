# 🌟 ImgToPassword 🌟
This is a small C program that can make any type of files your password vault.

## 🚀 Building the Project

To build the project, you can use the provided `Makefile`. By default, it uses `clang` as the compiler, but you can choose a different compiler if you prefer.

## Usage

To use the `PasswordHasher` program, you need to provide the following command-line options:

- `-f <file>`: Path to the password file.
- `-c <char_array>`: Characters that can be in the password (i.e. the password alphabet).
- `-l <length>`: Desired password length.

Here's an example command to run the program:

```bash
./PasswordHasher -f passwords.txt -c "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz{}|–…·.,<>+~*#" -l 12
```

Make sure to replace `passwords.txt` with the path to your password file, and adjust the character array and length according to your requirements.

That's it! The program will generate a password based on the provided options.

Enjoy using `PasswordHasher` to secure your files! 🔒

