#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include <list.h>
#include "threads/thread.h"
#include "threads/synch.h"

tid_t process_execute (const char *cmd_line);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);

struct start_process_info {
  char *cmd_line;
  bool load_success;
  struct semaphore awake_parent;
  struct parent_child *parent_child;
};

#endif /* userprog/process.h */
