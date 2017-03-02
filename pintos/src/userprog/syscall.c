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
#include "userprog/process.h"
#include "userprog/pagedir.h"
#include "threads/vaddr.h"

static void syscall_handler (struct intr_frame *);
bool is_valid_ptr (const void *ptr);

void
syscall_init (void)
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED)
{
  int *s = f->esp;
  if (!is_valid_ptr(s)) {
    exit(-1);
  }
  else {
    switch (*s) {
      case 0: /* HALT */
        halt();
        break;
      case 1: /* EXIT */
        if (!is_valid_ptr(s+1)) {
          exit(-1);
        }
        exit (*(s+1));
        break;
      case 2: /* EXEC */
        if (!is_valid_ptr(s+1)) {
          exit(-1);
        }
        f->eax = exec ((const char *)*(s+1));
        break;
      case 3: /* WAIT */
        if (!is_valid_ptr(s+1)) {
          exit(-1);
        }
        f->eax = wait(*(s+1));
        break;
      case 4: /* CREATE */
        if (!is_valid_ptr(s+1) || !is_valid_ptr(s+2)) {
          exit(-1);
        }
        f->eax = (uint32_t) create((const char *)*(s+1), *(s+2));
        break;
      case 5: /* REMOVE */
        break;
      case 6: /* OPEN */
        if (!is_valid_ptr(s+1)) {
          exit(-1);
        }
        f->eax = open((const char *)*(s+1));
        break;
      case 7: /* FILESIZE */
        break;
      case 8: /* READ */
        if (!is_valid_ptr(s+1) || !is_valid_ptr(s+2) || !is_valid_ptr(s+3)) {
          exit(-1);
        }
        f->eax = read(*(s+1),(void *) *(s+2), *(s+3));
        break;
      case 9: /* WRITE */
        if (!is_valid_ptr(s+1) || !is_valid_ptr(s+2) || !is_valid_ptr(s+3)) {
          exit(-1);
        }
        f->eax = write(*(s+1), (const void *) *(s+2), *(s+3));
        break;
      case 10: /* SEEK */
        break;
      case 11: /* TELL */
        break;
      case 12: /* CLOSE */
        if (!is_valid_ptr(s+1)) {
          exit(-1);
        }
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
}

bool is_valid_ptr (const void *ptr) {
  return (is_user_vaddr (ptr) && (pagedir_get_page (thread_current()->pagedir, ptr) != NULL));
}

bool is_valid_string (const char *ptr) {
  char current_character = *ptr;
  do {
      if (!is_valid_ptr(ptr)) {
        return false;
      }
      else {
        current_character = *ptr;
        ptr ++;
      }
    } while (current_character != '\0');
  return true;
}

bool is_valid_buffer (const void *buf, unsigned int size) {
  unsigned int i;

  for (i = 0; i < size; i++) {
    if (!is_valid_ptr(buf)) {
      return false;
    }
    else {
      buf++;
    }
  }
  return true;
}


void halt(void) {
  power_off();
}

bool create (const char *file, unsigned int initial_size) {
  if (!is_valid_ptr(file) || !is_valid_string(file)) {
    exit(-1);
  }
  return filesys_create (file, initial_size);
}

int open (const char *file) {
  if (!is_valid_ptr(file) || !is_valid_string(file)) {
    exit(-1);
  }
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
  file_close (cur_file);
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
  if (!is_valid_ptr(buffer) || !is_valid_buffer(buffer, size)) {
    exit(-1);
  }
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
  if (!is_valid_buffer(buffer, size)) {
    exit(-1);
  }
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
  struct thread *t = thread_current();
  t->exit_status = status;
  if(t->parent != NULL) {
    t->parent->exit_status = status;
  }
  thread_exit ();
}


pid_t exec (const char *cmd_line) {
  if (!is_valid_ptr(cmd_line) || !is_valid_string(cmd_line)) {
    exit(-1);
  }
  return process_execute(cmd_line);
}

int wait (pid_t pid) {
  return process_wait(pid);
}
