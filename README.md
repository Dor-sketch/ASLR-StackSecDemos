# Security Demonstrations

This repository contains demonstrations of various security concepts, emphasizing on Address Space Layout Randomization (ASLR) and stack overflow vulnerabilities.

## 1. ASLR Demonstration

### Overview
ASLR is a computer security technique that randomizes the memory addresses used by processes, making it harder for an attacker to predict the location of specific functions or buffers they might target.

### Features

- **Toggle ASLR**: Using the program `aslr_examp`, you can turn ASLR on or off on a Linux system. This is useful to visualize the effects of ASLR on memory address allocations.
  
- **Memory Address Visualization**: The program `print_mem` is a helper utility that prints the memory address of a dynamically allocated variable. It is used to visually demonstrate the effect of enabling or disabling ASLR.

### Compilation and Usage

To compile the ASLR demonstration:

```bash
make aslr_examp print_mem
```

Run the ASLR demo:

```bash
./aslr_examp
```

## 2. Stack Overflow Demonstration

### Overview
This program, named `stack_demo`, showcases a stack overflow vulnerability by manipulating the virtual pointer (`vptr`) of a class, leading to a redirection in its virtual function calls.

### Compilation and Usage

To compile the stack overflow demonstration:

```bash
make stack_demo
```

Run the program:

```bash
./stack_demo
```

## Compilation for All Demonstrations

To compile all the demonstrations at once, simply use:

```bash
make
```

## Cleaning Up

To clean up and remove the compiled binaries:

```bash
make clean
```

## Contributing

If you'd like to contribute to this project, please fork the repository and submit a pull request.

## Acknowledgment

The code examples in this repository are inspired by and built upon concepts studied in the Open University course "Defensive System-Programming (20937)".

## Disclaimer

The code in this repository is strictly for educational purposes. The demonstrated vulnerabilities aim to raise awareness and understanding of potential security risks. Do not use the code maliciously or without proper understanding. Ensure that you have necessary permissions before making system-level changes, such as modifying ASLR settings.

## License

This project is open-source and is licensed under the MIT License.
