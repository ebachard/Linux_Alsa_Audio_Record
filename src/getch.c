
/* File getch.c, belongs to audio_record project
 * Author : Eric Bachard / dimanche 27 mars 2016, 13:24:52 (UTC+0200)
 * This document is under GPL v2 License. See : http://www.gnu.org/licenses/gpl-2.0.html
 */


#include <stdio.h>
/* rien */
/*#include <iostream>*/

#include "getch.h"
#include <termios.h>
#include <stdlib.h>
#include <linux/fd.h>

/* D'après POSIX.1-2001 */
/* #include <sys/select.h> */

/* autres implémentations */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

char getch()
{
    fd_set set;
    struct timeval timeout;
    int rv;
    char buff = 0;
    int len = 1;
    int filedesc = 0;
    FD_ZERO(&set);
    FD_SET(filedesc, &set);

    timeout.tv_sec = 0;
    timeout.tv_usec = 1000;
    rv = select(filedesc + 1, &set, NULL, NULL, &timeout);

    struct termios old;

    /*
     * Get the current settings. This saves us from
     * having to initialize a struct termios from scratch.
     * Source : https://en.wikibooks.org/wiki/Serial_Programming/termios
     */

    if(tcgetattr(STDIN_FILENO, &old) < 0)
    {
        perror("stdin");
        return EXIT_FAILURE;
    }


    tcgetattr(filedesc, &old);
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;


    tcsetattr(filedesc, TCSANOW, &old);

    if(rv != -1)
    {
      int anErr = read(filedesc, &buff, len );

      if (anErr < 0)
        fprintf(stdout, "read issue");
    }

    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;

    tcsetattr(filedesc, TCSADRAIN, &old);

    return (buff);
}
