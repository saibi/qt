#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include "led.h"

static int 	led_fd = -1;

void init_led(void)
{
	led_fd = open("/sys/class/leds/green_ph20_led1/brightness", O_WRONLY);
	if(led_fd < 0)
		fprintf(stderr, "led Open Error\n");
}

void close_led(void)
{
	g_led_off();
	close(led_fd);
	led_fd = -1;
}

void g_led_on(void)
{
	if ( led_fd >= 0 )
		if ( write(led_fd, "0", 1) < 0 )
			fprintf(stderr, "led write Error\n");

}

void g_led_off(void)
{
	if ( led_fd >= 0 )
		if ( write(led_fd, "1", 1) < 0 )
			fprintf(stderr, "led write Error\n");
}
