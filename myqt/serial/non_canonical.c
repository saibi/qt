#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#define BAUDRATE B9600
#define MODEMDEVICE "/dev/ttyUSB0"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

volatile int STOP=FALSE; 

int main(int argc, char *argv[])
{
	int fd, res;
	struct termios oldtio,newtio;
	char buf[8];

	fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY | O_NDELAY ); 
	if (fd <0) {perror(MODEMDEVICE); exit(-1); }

	printf("non canonical serial read test.\n");
	printf("fd = %d\n", fd);

	tcgetattr(fd,&oldtio); /* save current port settings */

	bzero(&newtio, sizeof(newtio));
	newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD ; // | CRTSCTS ;
	newtio.c_iflag = IGNPAR | IGNBRK;
	newtio.c_oflag = 0;

	/* set input mode (non-canonical, no echo,...) */
	newtio.c_lflag = 0;

	newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
	newtio.c_cc[VMIN]     = 1;   /* blocking read until 5 chars received */

	tcflush(fd, TCOFLUSH);
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd,TCSANOW,&newtio);


	while (STOP==FALSE) {       /* loop for input */
		res = read(fd, buf, sizeof(buf)-1);   /* returns after 5 chars have been input */
		if ( res < 0 ) { 
			continue;
		}
		buf[res]=0;               /* so we can printf... */
		printf(":%s:%d\n", buf, res);
		if (buf[0]=='z') STOP=TRUE;
	}
	tcsetattr(fd,TCSANOW,&oldtio);
	return 0;
}

