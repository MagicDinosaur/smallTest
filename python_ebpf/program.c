#include <uapi/linux/ptrace.h>
#include <linux/sched.h>

BPF_PERF_OUTPUT(events);

struct data_t{
    u32 pid;
    u32 ppid;
    char comm[TASK_COMM_LEN];
    char filename[128];

};

int trace(struct pt_regs *ctx, const char __user *filename) {
    struct data_t data = {};

    data.pid = bpf_get_current_pid_tgid() >> 32 ; //process ID
    struct task_struct *task = (struct task_struct *)bpf_get_current_task();
    data.ppid = task->real_parent->tgid;

    
    bpf_get_current_comm(&data.comm, sizeof(data.comm)); //p's name
    bpf_probe_read_user(data.filename, sizeof(data.filename), filename); //read filename = command, I just like to put as this


    events.perf_submit(ctx, &data, sizeof(data));

    return 0;



}

