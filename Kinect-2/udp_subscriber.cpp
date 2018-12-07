#include "udp_subscriber.h"
#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include <sys/types.h>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <iterator>
#include <iomanip>
#pragma comment(lib, "Ws2_32.lib")

udp_subscriber::udp_subscriber(int port)
{
	
	this->sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	this->serv.sin_family = AF_INET;
	this->serv.sin_port = htons(port);

	InetPton(this->serv.sin_family, _T("127.0.0.1"), &this->serv.sin_addr.s_addr);

	int result = connect(this->sockfd, (struct sockaddr*)&this->serv, sizeof(this->serv));
	__yal_log(__YAL_DBG, "Connected? %i\n", result);
}


udp_subscriber::~udp_subscriber()
{
	closesocket(this->sockfd);
}

void udp_subscriber::callback(normalized_head_data hd)
{
	//rot.axis[i], pos.axis[i]
	normalized_rot_t rot = hd.get_rot();
	normalized_pos_t pos = hd.get_pos();

	double p[6] = { pos.axis[0], pos.axis[1], pos.axis[2], rot.axis[0], rot.axis[1], rot.axis[2] };

	int result = sendto(this->sockfd, (char*)&p[0], sizeof(p), 0, (const struct sockaddr *)&this->serv, sizeof(this->serv));
	__yal_log(__YAL_DBG, "Sending (%i)\n", result);
}
