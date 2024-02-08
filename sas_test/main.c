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

#define CAN_IFACE	"vcan0"

u_int8_t sas_crc(u_int8_t *data, int len);

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

	strcpy(ifr.ifr_name, CAN_IFACE );
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

		if (nbytes < 0) {
			perror("Read");
			return 1;
		}

		float angle = (float)(((frame.data[0] << 8)|frame.data[1]) - 0x8000) / 10.0;
		float speed = (float)(((frame.data[2] << 8)|frame.data[3]) - 0x8000) / 10.0;

		printf("%.02f;%.02f ",angle,speed);
		
		if(sas_crc(frame.data,6) != frame.data[6]){
			printf("CRC not match. Calc: 0x%0X Get: 0x%0X\n",sas_crc(frame.data,6),frame.data[6]);
		}else{
			
			printf("CRC Ok\n");
		}

	}


	if (close(s) < 0) {
		perror("Close");
		return 1;
	}

	return 0;

}


u_int8_t sas_crc(u_int8_t *data, int len){

	u_int8_t summ = 0;

	for(u_int32_t i=0; i<len; i++){
		summ += *data++;
	}

	summ = 255 - summ;
	
	return summ;
}