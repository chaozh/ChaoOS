#include <types.h>
#include <x86.h>
#include <sched.h>

#include <init_task.h>

struct task_struct init_task = INIT_TASK(init_task);
struct tss_desc tss;

//fork.c
struct task_struct *tasks[NPROC] = {NULL, };
struct task_struct *_current = NULL;

void swapper_init() {
	_current = tasks[0] = &init_task;
	//init tss for switch
	tss.ss0 = KERN_DS;
	tss.esp0 = KSTACK0;
}
//return pid
uint kernel_thread(void (*func)(void* arg), void* arg, uint flags) {
	uint pid;
	
	//pid = find_pid();
	if (pid == 0){
		//panic
	}
	
	return pid;
}
