# AOS_SystemProgramming
# File Reversal & Verification in C/C++

## 📌 Overview

This REPO consists of two programs:

1. **mycode:** File reversal based on different flags, with progress percentage printing.
2. **verification:** Verification of the file reversal output, file permissions, and directory permissions.

Both programs are designed to handle **large files (>1 GB)** efficiently, even when file size exceeds system RAM.

---

## 📂 mycode: File Reversal Operations

### General Requirements

- **Progress Display:** The percentage of the file written is printed to the console during writing and **overwritten in-place** (not printed multiple times).
- **Directory Permissions:** Created directory has read, write, and execute permissions for the user.
- **File Permissions:** Created file has read and write permissions for the user.
- **Efficiency:** Works with files larger than system RAM.

---

### Command-Line Usage

```bash
# Compilation
gcc Q1.c    # or g++ Q1.cpp

# Execution
./a.out <input file> <flag> [additional arguments]
```

#### **Flag 0: Block-wise Reversal**

- Reverses the file in **fixed-size blocks** (given as the 3rd argument).
- Blocks remain in the original order.
- Example:
  ```
  Input:  ABCDEFGH
  Block:  4
  Output: DCBAHGFE
  ```
- Usage:
  ```bash
  ./a.out input.txt 0 <block size>
  Output file: Assignment1/0<filename>
  ```

#### **Flag 1: Full File Reversal**

- Entire file is reversed globally.
- Example:
  ```
  Input:  Hello, World!
  Output: !dlroW ,olleH
  ```
- Usage:
  ```bash
  ./a.out input.txt 1
  Output file: Assignment1/1<filename>
  ```

#### **Flag 2: Partial Range Reversal**

- Reverses **[0, start index]** and **[end index, EOF]**.
- Keeps **[start index+1, end index]** unchanged.
- Indices are **zero-based**.
- Example:
  ```
  Input:  Hello, World!
  Start:  2
  End:    7
  Output: eHllo, W!dlro
  ```
- Usage:
  ```bash
  ./a.out input.txt 2 <start index> <end index>
  Output file: Assignment1/2<filename>
  ```

---

### **mycode Code Structure**

- **main()** – Parses arguments, switches between flag modes, calls respective functions.
- **flag_0_1()** – Handles block-wise and full-file reversal.
- **flag_2()** – Handles partial range reversal.
- **rev_file()** – Reads, reverses, and writes data in chunks.
- **rev_block()** – Reverses a buffer in place.

---

## 📂 verification: File & Permission Verification

### Purpose

- Verify **directory** and **file permissions**.
- Validate that the new file matches the expected reversal based on the flag used.
- Check that both files have the **same size**.

---

### Command-Line Usage

```bash
./a.out <new file path> <old file path> <directory> <flag> [<blockSize>|<start> <end>]
```

#### Arguments per Flag

- **Flag 0:** 5 arguments (`flag=0`, block size)
- **Flag 1:** 4 arguments
- **Flag 2:** 6 arguments (`flag=2`, start, end)

---

### Verification Logic

- **Flag 0:**
  - Read both files in given block size.
  - Each block in new file must be the reverse of corresponding block in original.
- **Flag 1:**
  - Compare original from start with reversed chunks from end of new file.
- **Flag 2:**
  - Verify `[0, start]` reversed.
  - Verify `[start+1, end]` identical.
  - Verify `[end+1, EOF]` reversed.

---

### Output Format Example

```
Directory is created: Yes
Whether file contents are correctly processed: Yes
Both Files Sizes are Same: Yes
User has read permissions on newfile: Yes
User has write permission on newfile: Yes
User has execute permission on newfile: No
Group has read permissions on newfile: No
...
(30 lines total for user/group/others permissions for both files and directory)
```

---

### **verification Code Structure**

- **main()** – Parses arguments, chooses verification based on flag, checks directory existence.
- **flag_0_1()** – Verifies block-wise and full-file reversals.
- **flag_2()** – Verifies partial range reversal.
- **verify_rev()** – Checks if content matches expected reversal.
- **rev_block()** – Reverses a buffer.
- **comp()** – Compares two buffers.
- **file_dir_ver()** – Prints permission checks for user/group/others.

---

## 🛠 Compilation & Execution Examples

```bash
# Q1 Examples
./a.out bigfile.txt 0 4096
./a.out bigfile.txt 1
./a.out bigfile.txt 2 1000 5000

# Q2 Examples
./a.out Assignment1/0bigfile.txt bigfile.txt Assignment1 0 4096
./a.out Assignment1/1bigfile.txt bigfile.txt Assignment1 1
./a.out Assignment1/2bigfile.txt bigfile.txt Assignment1 2 1000 5000
```

---

## 📜 Notes

- Designed for **large files (>1 GB)**.
- Works with **files larger than available RAM**.
- Uses `lseek()` to minimize memory usage.
- Progress is displayed **in-place** for smooth updates.
- Directory and file permission handling uses **POSIX APIs** (`chmod`, `stat`).

---

## 👨‍💻 Author

**Akula Reddy**  
C / C++ Systems Programming – File Handling & Permissions
