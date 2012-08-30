//
// process.h
//
// written by sjrct
//

#ifndef PROCESS_H
#define PROCESS_H

#include "kernobj.h"

enum {
	THRD_STATE_READY = 0,
	THRD_STATE_SETUP,
	THRD_STATE_RUNNING,
	THRD_STATE_WAITING,
	THRD_STATE_STOPPED,
};

enum {
	PROC_PRIV_NORMAL,
	PROC_PRIV_ELEVATED,
	PROC_PRIV_SUPER,
};

#define PROC_PRIORITY_IDLE 0

//
// init_processes: Initializes the process-related data
//
void init_processes();

//
// start_timer: Starts the timer used for context switching
//
void start_timer();

//
// spawn_process: Spawns a new process
//
// Parameters:
//   uchar *    code_and_data
//   ulong      code_size
//   ulong      data_size
//   ulong      bss_size
//   kern_obj * parent
//   uchar      privledge
//   uchar      priority
//
// The parent process can be NULL
//   
kern_obj * spawn_process(uchar *, ulong, ulong, ulong, kern_obj *, uchar, uchar);

//
// spawn_thread: Spawns a new thread from a process
//
// Parameters:
//   kern_obj * process
//   int (*function)()
//
// Returns: Thread object
//
// The function is a pointer valid within the given process
//
kern_obj * spawn_thread(kern_obj *, int (*)());

#endif
