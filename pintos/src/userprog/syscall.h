#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
#include <stdbool.h>
#include "lib/user/syscall.h"

void syscall_init (void);

void halt(void);

bool create (const char *file, unsigned initial_size);

int open (const char *file);

void close (int fd);

int read (int fd, void *buffer, unsigned size);

int write (int fd, const void *buffer, unsigned size);

pid_t exec (const char *cmd_line);

void exit (int status);

int wait (pid_t pid);

bool is_valid_ptr (const void *ptr);

bool is_valid_string (const char *ptr);

bool is_valid_buffer (const void *buf, unsigned size);

void seek (int fd, unsigned position);

unsigned tell (int fd);

int filesize (int fd);

bool remove (const char *file_name);

#endif /* userprog/syscall.h */
