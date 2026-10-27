# EmbedKit - Embedded C Utility Library

## Name
Shubhangi Ranewad

## Module: Ring Buffer

Implemented an 8-byte circular buffer in standard C99 using uint8_t.

The module supports:
- Initialization
- Writing a byte
- Reading a byte
- Counting stored bytes
- Checking full and empty states

The implementation prevents writing when the buffer is full and reading when it is empty. It preserves FIFO (First In, First Out) order.

## Bonus Optimization

Bitwise AND is used instead of modulo for index wrap-around:

`index = (index + 1U) & (BUFFER_SIZE - 1U)`

This works because the buffer size is 8, which is a power of two. For other buffer sizes, this optimization is not generally valid.

## Requirements
- GCC compiler
- C99-compatible compiler

## Build

```bash
gcc -Wall -std=c99 ringbuf.c -o ringbuf
```

## Run

Windows PowerShell:

```powershell
.\ringbuf.exe
```

Linux/macOS:

```bash
./ringbuf
```

## Test Results

- Buffer fills to 8 bytes.
- Write to a full buffer fails.
- First three bytes are read in FIFO order.
- Three new bytes are written successfully.
- Remaining bytes are read in FIFO order.
- Read from an empty buffer fails.