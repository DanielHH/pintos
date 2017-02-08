#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"
#include "threads/synch.h"

tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);

struct parent_info {
  thread *parent_thread;
  char *fn;
  struct semaphore s;
}

#endif /* userprog/process.h */
