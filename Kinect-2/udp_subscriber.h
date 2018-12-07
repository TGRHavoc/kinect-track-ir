#include "subscriber.h"
#include "normalized_head_data.h"
#include <winsock2.h>


#ifndef __UDP_SUBSCRIBER__
#define __UDP_SUBSCRIBER__


class udp_subscriber :
	public subscriber<normalized_head_data>
{
public:
	udp_subscriber(int port);
	~udp_subscriber();
	void callback(normalized_head_data hd);

private:
	int sockfd;
	struct sockaddr_in serv;
};

#endif 