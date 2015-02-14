#ifndef INIT_TASK_H
#define INIT_TASK_H

#define INIT_TASK(tsk)	\
{			\
	.stat = TASK_RUNNING,	\
	.pid = 0,	\
	.parent_pid = 0,	\
	.flags = SLOAD,	\
	.parent = &tsk,	\
	.real_parent = &tsk,	\
			\
	.cpu = 0,	\
	.prio = 0,	\
	.nice = 20, 	\
			\
	.gid = 0,	\
	.sid = 0,	\
			\
}

#endif
