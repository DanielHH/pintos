#include "userprog/syscall.h"
#include <stdio.h>
#include <string.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"
#include "filesys/filesys.h"
#include "threads/malloc.h"
#include "devices/input.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void)
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED)
{
  int *s = f->esp;
  switch (*s) {
    case 0: /* HALT */
      halt();
      break;
    case 1: /* EXIT */
      exit (*(s+1));
      break;
    case 2: /* EXEC */
      break;
    case 3: /* WAIT */
      break;
    case 4: /* CREATE */
      f->eax = (uint32_t) create((const char *)*(s+1), *(s+2));
      break;
    case 5: /* REMOVE */
      break;
    case 6: /* OPEN */
      f->eax = open((const char *)*(s+1));
      break;
    case 7: /* FILESIZE */
      break;
    case 8: /* READ */
      f->eax = read(*(s+1),(void *) *(s+2), *(s+3));
      break;
    case 9: /* WRITE */
      f->eax = write(*(s+1), (const void *) *(s+2), *(s+3));
      break;
    case 10: /* SEEK */
      break;
    case 11: /* TELL */
      break;
    case 12: /* CLOSE */
      close(*(s+1));
      break;
    case 13: /* MMAP */
      break;
    case 14: /* MUNMAP */
      break;
    case 15: /* CHDIR */
      break;
    case 16: /* MKDIR */
      break;
    case 17: /* READDIR */
      break;
    case 18: /* ISDIR */
      break;
    case 19: /* INUMBER */
      break;
  }
}

void halt(void) {
  power_off();
}

bool create (const char *file, unsigned initial_size) {
  return filesys_create (file, initial_size);
}

int open (const char *file) {
  unsigned i;
  struct thread *t = thread_current();
  struct file *cur_file;
  cur_file = filesys_open (file);
  if (cur_file != NULL) {
    for(i=2; i<130; i++) {
      if (t->open_files[i-2] == NULL) {
        t->open_files[i-2] = cur_file;
        return i;
      }
    }
  }
  return -1;
}

void close (int fd) {
  if(fd > 1 && fd < 130) {
    struct thread *t = thread_current();
    file_close (t->open_files[fd-2]);
    t->open_files[fd-2] = NULL;
  }
}

int read (int fd, void *buffer, unsigned size) {
  if(fd == 0) {
    int read_bytes = 0;
    unsigned i;
    for(i = 0; i < size; i++) {
      uint8_t read_byte = input_getc ();
      buffer = &read_byte;
      read_bytes++;
      buffer += 1;
    }
    return read_bytes;
  }
  else if(fd > 1 && fd < 130) {
    struct thread *t = thread_current();
    if(t->open_files[fd-2] != NULL) {
      return (int) file_read (t->open_files[fd-2], buffer, (off_t) size);
    }
  }
  return -1;
}

int write (int fd, const void *buffer, unsigned size) {
  if (fd == 1) {
    char *char_buffer = (char *) buffer;
    if(strlen(char_buffer) < size) {
      size = strlen(char_buffer);
    }
    if(size > 256) {
      size = 256;
    }
    putbuf (char_buffer, size);
    return (size);
  }
  else if (fd > 1 && fd < 130) {
    struct thread *t = thread_current();
    if(t->open_files[fd-2] != NULL) {
      return (int) file_write (t->open_files[fd-2], buffer, (off_t) size);
    }
  }
  return -1;
}

void exit (int status) {
  thread_exit ();
}
