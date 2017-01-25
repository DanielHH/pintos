#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"

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
  switch (f->&esp) {
    case 0: /* HALT */
      halt_handler(void);
      break;
    case 1: /* EXIT */
      break;
    case 2: /* EXEC */
      break;
    case 3: /* WAIT */
      break;
    case 4: /* CREATE */
      create_handler(f->&(esp+4), );
      break;
    case 5: /* REMOVE */
      break;
    case 6: /* OPEN */
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

void halt_handler(void) {
  power_off();
}

bool create (const char *file, unsigned initial_size) {

}
