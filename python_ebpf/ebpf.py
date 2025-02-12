from bcc import BPF

import os

with open("program.c", "r") as f:
    code = f.read()


bpf = BPF(text=code)

bpf.attach_kprobe(event = "do_sys_open", fn_name ="trace")
# bpf.attach_tracepoint(tp="syscalls:sys_enter_execve", fn_name="trace")

def print_event(cpu, data, size):

    event = bpf["event"].event(data)
    print(f"Process: {event.comm.decode()} (PID: {event.pid}, PPID: {event.ppid}) Executed: {event.filename.decode()}")


bpf["events"].open_perf_buffer(print_event)

print("Monitoring process executions... (Press CTRL+C to stop)")

# Poll events
try:
    while True:
        bpf.perf_buffer_poll()
except KeyboardInterrupt:
    print("\nStopping eBPF monitoring.")