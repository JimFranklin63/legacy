#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include <can.h>
#include <lap.h>

#include "cmds-cantemp.h"





void cmd_cantemp(int argc, char *argv[]) 
{
	unsigned char addr;
	can_message *msg;
	msg = can_buffer_get();
	can_message *result;

	if (argc != 2) goto argerror;
       	if (sscanf(argv[1], "%x", (unsigned int*)&addr) != 1)
		goto argerror;


	msg->addr_src = 0x00;
	msg->addr_dst = addr;
	msg->port_src = 0x00;
	msg->port_dst = 0x10;
	msg->dlc      = 1;
	msg->data[0]  = 0;
	

	can_transmit(msg);

	for (;;)
	{
		result = can_get();

		if (result->addr_src == addr)
		{
			printf("Temp is %d\n", result->data[0]);
			return;
		}
	}

	return;
argerror:
	debug(0, "cantemp <addr>");
};
