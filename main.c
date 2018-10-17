#include "window.h"
#include <stdlib.h>

int main() 
{
	char* buffer = malloc(640*480*3);
	
	int i;
	for(i = 0; i < 640*480*3; i++)
		buffer[i] = rand()%512 - 256;

	create_window();
	set_buffer(buffer, 640, 480);
	for(;;) { 
		if(get_input() == -1) break;
		draw_image();
	} 
	destroy_window();
}
