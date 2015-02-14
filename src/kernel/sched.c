#include <types.h>
#include <x86.h>
#include <sched.h>

void __switch_to(struct thread_info *prev, struct thread_info *next) {
	struct task_struct *from = prev->task, *to = next->task;
	tss.esp0 = to->stack; //TODO:some problem
	_current = to;
	//call nasm
	do_switch(&(from->thread), &(to->thread));
}
