#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"
#include "filesys/filesys.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void)
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED)
{
  printf ("system call!\n");
  int *s = f->esp;
  switch (*s) {
    case 0: /* HALT */
      halt(void);
      break;
    case 1: /* EXIT */
      break;
    case 2: /* EXEC */
      break;
    case 3: /* WAIT */
      break;
    case 4: /* CREATE */
      f->eax = create(*(s+4), *(s+8));
      break;
    case 5: /* REMOVE */
      break;
    case 6: /* OPEN */
      f->eax = open(*(s+4));
      break;
    case 7: /* FILESIZE */
      break;
    case 8: /* READ */
      break;
    case 9: /* WRITE */
      break;
    case 10: /* SEEK */
      break;
    case 11: /* TELL */
      break;
    case 12: /* CLOSE */
      close(*(s+4);
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
  thread_exit ();
}

void halt(void) {
  power_off();
}

bool create (const char *file, unsigned initial_size) {
  return filesys_create (file, initial_size);
}

int open (const char *file) {
  int i;
  thread *t = thread_current();
  for(i=2; i<128; i++) {
    if (t->open_files[i] == NULL) {
      file *cur_file = (file*) malloc(sizeof(file));
      cur_file = filesys_open (file);
      file_desc *fd = (file_desc*) malloc(sizeof(file_desc));
      fd->fd = i;
      fd->file = cur_file;
      t->open_files[i] = fd;
      return i;
    }
  }
  return -1;
}

void close (int fd) {
  if(fd > 1 && fd < 128) {
    thread *t = thread_current();
    file_close (t->open_files[fd]->file);
    free(t->open_files[fd]);
    t->open_files[fd] = NULL;
  }
}
