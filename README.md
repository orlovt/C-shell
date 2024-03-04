# C-shell

## Description
This repository contains a simple low level implementation of a Unix shell in C. It includes basic shell commands like `cd`, `ls`, and `wc`, each implemented in their own C files. This implementation also includes the `pip` and `STDOUT` redirection functionality. The `shell.c` file contains the main shell loop and logic. This project is suitable for educational purposes, for those interested in understanding how shell commands are implemented at a lower level.

## Installation
To install, clone the repository and use the provided Makefile:
```
git clone https://github.com/orlovt/C-shell.git
cd C-shell
make
```

## Usage
After compiling, you can run the shell executable:
```
./shell
[C-shell]/Users/path/to/repo/C-shell$
```
From there, you can use the implemented commands within this custom shell environment.

```
[C-shell]/Users/path/to/repo/C-shell$ ls -R \path/to/dir
```

```
[C-shell]/Users/path/to/repo/C-shell$ ls -R | wc > out.txt
```

## Contributing
Contributions are welcome! Please feel free to submit pull requests or open issues for any improvements or bug fixes.
