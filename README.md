# Todo List in C

I've created this project as a result of a course I had with ChatGPT. I did not copy paste anything. I tried to follow the instructions from the GPI while applying my knowledge of software architecture. The code is full with my comments so I can recap and remember concepts.

## Getting Started

Follow the instructions below to install dependencies and run the project on **Windows**, **macOS**, and **Linux**.

---

### 1. Clone the Repository

```bash
git clone https://github.com/adrianosilveira13/todo_app_c.git
cd todo_app_c
```

---

### 2. Install Dependencies

#### Linux / macOS

You need to have `gcc` and `make` installed.

- **Debian/Ubuntu:**
    ```bash
    sudo apt update
    sudo apt install build-essential
    ```

- **Fedora:**
    ```bash
    sudo dnf groupinstall "Development Tools"
    ```

- **macOS (with Homebrew):**
    ```bash
    brew install gcc make
    ```

#### Windows

- Install [MinGW](http://www.mingw.org/) or [TDM-GCC](https://jmeubank.github.io/tdm-gcc/) for `gcc`.
- Add `gcc` and `make` to your system PATH.
- Alternatively, use [WSL](https://docs.microsoft.com/en-us/windows/wsl/) to run Linux commands.

---

### 3. Build and Run the Project

Use the provided `Makefile` to build the project:

```bash
make
```

This will compile the source code and generate the executable.

To run the application:

```bash
./todo_list
```
(On Windows, the executable will be `todo_list.exe`)

---

## About the Code

- The code contains many comments written by the creator, summarizing their understanding and helping to reinforce concepts.
- This project is intended as a learning exercise and may not follow all best practices.

---

## License

This project is for educational purposes.

---