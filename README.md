# Multithreaded Download Manager in C

## 📌 Project Overview

This is a **Multithreaded Download Manager** written in C that downloads files from a given URL using multiple threads. 
The program splits the file into chunks and downloads them concurrently, improving download speed and efficiency.

## 🚀 Features

- Downloads files using **multiple threads**
- Uses **libcurl** for HTTP/HTTPS requests
- Supports **resume and chunked downloads**
- Merges downloaded parts into a single file
- Implements **error handling** for invalid URLs and connection issues

## 📂 Project Structure

```
📁 Multithreaded_Downloader
├── downloader.c         # Main source code
├── README.md            # Project documentation
└── Makefile             # (Optional) Build script
```

## 🛠️ Prerequisites

Make sure you have the following installed:

- **GCC Compiler** (`gcc`)
- **libcurl** (`libcurl-dev` or `libcurl-devel`)
- **POSIX Threads (pthreads)** (comes with Linux/macOS)

### Installing `libcurl`

#### 🔹 On Ubuntu/Debian:

```sh
sudo apt update && sudo apt install libcurl4-openssl-dev
```

#### 🔹 On Fedora:

```sh
sudo dnf install libcurl-devel
```

#### 🔹 On macOS (Homebrew):

```sh
brew install curl
```

#### 🔹 On Windows (MSYS2):

```sh
pacman -S mingw-w64-x86_64-curl
```

## 🔧 Compilation & Running

### 1️⃣ Compile the Program

```sh
gcc -o downloader downloader.c -lpthread -lcurl
Example:
"gcc downloader.c -o downloader.exe -I"C:\vcpkg\installed\x64-windows\include" -L"C:\vcpkg\installed\x64-windows\lib" -lcurl -lpthread"
```

### 2️⃣ Run the Program

```sh
./downloader <URL> <NUMBER_OF_THREADS> <OUTPUT_FILENAME>
```

### Example:

```sh
./downloader https://example.com/file.zip 4 output.zip
```

This will download `file.zip` using **4 parallel threads** and save it as `output.zip`.

## ⚠️ Troubleshooting

### ❌ `fatal error: curl/curl.h: No such file or directory`

- Ensure `libcurl` is installed (`sudo apt install libcurl4-openssl-dev` on Linux).
- On Windows, add `C:/msys64/mingw64/include` to your compiler's include path.

### ❌ `Segmentation fault` or `Invalid memory access`

- Ensure enough **RAM** is available when using **high thread counts**.
- Check if the URL is **valid** and **accessible**.

### ❌ `Failed to retrieve file size`

- The **server may not support file size queries** (Content-Length missing).
- Try a different URL or **reduce the number of threads**.

## 📜 License

This project is **open-source** and can be used or modified freely.

---

✍ **Author:** Dogacan Kutlu\
📅 **Last Updated:13 March 2025



