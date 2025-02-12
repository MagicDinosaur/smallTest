For this one, a bit tricky since I'm using arm64. So setup a machine is needed. 

anddd, run the machine with sudo priviledge

!note: eBPF workflow:
An eBPF program (program.c), this will act as kernel mode instruction.
Compile it into BPF bytecode.
Load it into the Linux kernel via a system call (bpf()).The kernel verifies the program for safety (no loops, no memory corruption).The program attaches to hooks (e.g., syscalls, network events, function calls).
The program will run in the kernel and can send data to user space.
