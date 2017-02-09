#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include <list.h>
#include "threads/thread.h"
#include "threads/synch.h"

tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);

struct parent_info {
  struct thread *parent_thread;
  struct thread *child_thread;
  char *fn;
  struct semaphore s;
  bool load_success;
};

#endif /* userprog/process.h */
