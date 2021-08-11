/* Have a look at the LICENSE file for copyright and license details. */

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include <ncurses.h>
#include <menu.h>

#include "ssfm.h"
#include "config.h"


void 
setdir(const char * dir) {
    if ( chdir(dir) == -1 )
      die("Error: %s %s\n", dir, strerror(errno));
}

void
die(const char *errstr, ...) {
	va_list ap;

	va_start(ap, errstr);
	vfprintf(stderr, errstr, ap);
	va_end(ap);
	exit(1);
}

int
main(int argc, char * argv[])
{
  if (argc > 1)
    setdir(argv[1]);
  else if (Start_Dir != NULL)
    setdir(Start_Dir);
 
  system("bash");

  return 0;
}
