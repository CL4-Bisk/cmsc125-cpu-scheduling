# About `cpu-scheduling`

# Creator

- Keith Ashly Domingo (TheAshly/FakeThird)
- John Clyde Aparicio (Cl4-Bisk)

# Description

**`cpu-scheduling`** is currently an assignment focusing on implementing a discrete-event simulator that demonstrates how operating systems make scheduling decisions to optimize system performance in C. It implements CPU Scheduling in C and through this exercise gain hands-on experience with the algorithms covered in lectures and develop insight into the tradeoffs between different scheduling policies.

```
Created as a laboratory assignment for CMSC 125 Operating Systems.
```

# Features and Algorithms

**`cpu-scheduling`** offers certain features, which follows as:

- **Five Scheduling Algorithms:**
  1. FCFS — First Come First Served
  2. SJF — Shortest Job First
  3. STCF — Shortest Time to Completion First
  4. RR — Round Robin
  5. MLFQ — Multi-Level Feedback Queue

- **Performance Metrics per Algorithm:**
  1. Finish Time (FT)
  2. Turnaround Time (TT)
  3. Waiting Time (WT)
  4. Response Time (RT)
  5. Per-process and average values

- **Scaled ASCII Gantt chart** — Automatically scales to fit terminal width

- **Convoy effect detection** for FCFS

- **Context switch tracking** for all Algorithms

- **Flexible input** — Load workloads from a file or inline via command line

# Compilation and Usage

### I. Installing Windows Subsystem for Linux

(Skip to II if already installed or updated to WSL 2):

1. Ensure you are running Windows 10 version 2004 or higher, or Windows 11.
2. Ensure that Virtual Machine Platform, Windows Hypervisor Platform, and Windows Subsystem for Linux are all enabled under Windows Features.
3. Open PowerShell or Command Prompt as Administrator.
4. Run the following command to install WSL with Ubuntu as the default distribution:

```
wsl --install
```

5. Restart your computer, if prompted.
6. After a (possible) restart, Ubuntu will automatically launch and complete installation.
7. Create a UNIX username and password when prompted. Remember these credentials.
8. To verify installation, open a fresh command line and input the following:

```
wsl --list --verbose
```

You should see output showing Ubuntu with version 2 (WSL 2).

### II. Setting Up the C Development Toolchain

1. Open your WSL terminal (search for "Ubuntu" in the Start menu).
2. Update the package manager:

```
sudo apt update
```

3. Install essential build tools:

```
sudo apt install build-essential
```

4. Verify GCC installation:

```
gcc --version
```

You should see version information for GCC.

### III. Integrating Visual Studio Code with WSL

1. Download and install Visual Studio Code for Windows from the
   [official website](https://code.visualstudio.com/).
   (Skip to 6 if already installed VS Code and installed WSL and C/C++ extensions)
2. Open Visual Studio Code.
3. Under the Extensions tab, search for <strong>WSL</strong>.
4. Install the WSL extension by Microsoft.
5. Install the C/C++ extension by Microsoft.
6. Press <strong>Ctrl + Shift + P</strong> to open the command palette.
7. Type <strong>WSL: Connect to WSL</strong> and press Enter.
8. Visual Studio Code will reopen connected to your WSL environment.
9. Alternatively, you can try to navigate to your working folder through Windows File Explorer, open WSL in the directory, then type <strong>code .</strong> . If <strong>code .</strong> is not recognized, ensure VS Code was added to your Windows PATH during installation.
10. Visual Studio Code will open connected to the WSL environment with the directory as its working folder.
    You can verify this by checking the bottom-left corner of VS Code, which should display <strong>WSL: Ubuntu</strong>.

### IV. Running the Program in VS Code with WSL: Ubuntu

1. In VS Code connected to WSL, create a new folder for your projects with <strong>[folderName]</strong> as your desired name of the folder:

```
mkdir ~/[folderName]
cd ~/[folderName]
```

2. Open this folder in VS Code: File > Open Folder, then select your folder.
3. Open the integrated terminal in VS Code <strong>(`Ctrl + ``)</strong>.
4. Initialize Git before cloning the repository.

```
git init
```

5. Clone the repository using the web URL after initializing Git.

```
https://github.com/CL4-Bisk/cmsc125-cpu-scheduling.git
```

6. Open folder in VS Code to go in the `cmsc125-cpu-scheduling` folder repository using <strong>CRTL + K + O</strong>.

```
...
[folderName]
    |-- ...
    +-- cmsc125-cpu-scheduling/
        |-- Makefile
        |-- README.md
        |-- include/
        |   |-- process.h
        |   |-- scheduler.h
        |   |-- metrics.h
        |   +-- gantt.h
        |-- src/
        |   |-- main.c
        |   |-- process.c
        |   |-- fcfs.c
        |   |-- sjf.c
        |   |-- stcf.c
        |   |-- rr.c
        |   |-- mlfq.c
        |   |-- metrics.c
        |   |-- gantt.c
        |   +-- utils.c
        |-- tests/
        |   |-- workload1.txt
        |   +-- test_suite.sh
        +-- docs/
            +-- mlfq_design.md

```

You will see the contents inside the `cmsc125-cpu-scheduling` folder once inside the repository folder.

7. Run this command on your wsl terminal after cloning/downloading the repository:

```
make all
```

8. which you can now try and use for yourself, if you want to clean your files just run:

```
make clean
```

### V. Running the Program with In-line Arguments

1. In the wsl terminal you can run the program using, although it will cause an error for lacking arguments:

```bash
./schedsim
```

2. Certain arguments must be present while some are optional, e.g.:
   - `--algorithm=FCFS` (Must be present)
     - The algorithm you will use (FCFS, SJF, STCF, RR, MLFQ).
   - `--input=tests/workload1.txt` (Atleast one between Input or Processes must be present)
     - Location of the file to be read, it must follow the format (Name AT BT).
     - e.g. A 0 240
   - `--processes="A:0:240,B:10:180,C:20:150"` (Atleast one between Input or Processes must be present)
     - Command Line argument implementation that must follow the format ("Name:AT:BT, ..., Name:AT:BT")
     - e.g. "A:0:240,B:10:180,C:20:150"
   - `--quantum=20` (Optional)
     - For algorithms that uses quantum time (default: 30)

3. Now in the wsl terminal you can run the programing using and enjoy:

```bash
./schedsim --algorithm=RR --input=tests/workload1.txt --quantum=15
```

### VI. Additional tests
In case of additional tests for each schedulers, input the following:
```bash
make {type_of_scheduler}_input-{input_number}

ex. make fcfs_input-1

other types include before the _input-{input_number}:
fcfs
rr
sjf
stcf
mlfq
```

# Example Test Usage

```bash
./schedsim --algorithm=FCFS --input=tests/workload1.txt
```

```
Running FCFS Scheduler...

=== Metrics ===

Process    | AT    | BT    | FT    | TT    | WT    | RT
-----------|-------|-------|-------|-------|-------|------
A          | 0     | 240   | 240   | 240   | 0     | 0
B          | 10    | 180   | 420   | 410   | 230   | 230
C          | 20    | 150   | 570   | 550   | 400   | 400
D          | 25    | 80    | 650   | 625   | 545   | 545
E          | 30    | 130   | 780   | 750   | 620   | 620
-----------|-------|-------|-------|-------|-------|------
Average    |       |       |       | 515.0 | 359.0 | 359.0


=== Gantt Chart ===
(Each character = 13 time units)

|---------A---------|------B-------|-----C------|---D---|----E-----|
0                   240            420          570     650        780

[0000] START A
[0240] FINISH A
[0240] CONTEXT SWITCH A -> B
[0240] START B
[0420] FINISH B
[0420] CONTEXT SWITCH B -> C
[0420] START C
[0570] FINISH C
[0570] CONTEXT SWITCH C -> D
[0570] START D
[0650] FINISH D
[0650] CONTEXT SWITCH D -> E
[0650] START E
[0780] FINISH E
Warning: Process C is experiencing the convoy effect (waiting time: 400)
Warning: Process D is experiencing the convoy effect (waiting time: 545)
Warning: Process E is experiencing the convoy effect (waiting time: 620)
```

# Limitations and Bugs

### 1. Hardcoded MLFQ Config

- Queue parameters (quantum, allotment, boost period) are currently set in `main.c` and cannot be loaded from a config file via `--mlfq-config`.

### 2. `--compare` Mode not Implemented

- Running all algorithms in a single comparison table with averages values is not yet implemented.

### 3. Fixed Process limit

- Maximum of 100 processes per workload file as defined by `MAX_PROCESSES`.

### 4. Gantt Chart Scaling

- Very short process segments may appear compressed or merged at high scale factors.

# License

**Educational Use Only**

This project was created as an laboratory project for the CMSC 125 - Operating Systems.

- Free for **educational purposes** (learning, teaching, academic research)
- Free for **personal, non-commercial use**

**Copyright © 2026 Keith Ashly M. Domingo and John Clyde C. Aparicio**
