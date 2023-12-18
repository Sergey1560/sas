#include <string.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

int main(){
	int s;
	struct ifreq ifr;
	struct sockaddr_can addr;
	int nbytes;
	struct can_frame frame;

	if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("Socket");
		return 1;
	}

	strcpy(ifr.ifr_name, "can0" );
	ioctl(s, SIOCGIFINDEX, &ifr);

	memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Bind");
		return 1;
	}

	while(1){

		nbytes = read(s, &frame, sizeof(struct can_frame));

		float angle = (float)(((frame.data[0] << 8)|frame.data[1]) - 0x8000) / 10.0;
		float speed = (float)(((frame.data[2] << 8)|frame.data[3]) - 0x8000) / 10.0;

		printf("%.02f;%.02f\n",angle,speed);
		
		if (nbytes < 0) {
			perror("Read");
			return 1;
		}
	
	}


	if (close(s) < 0) {
		perror("Close");
		return 1;
	}

	return 0;

}