#include <chrono>
#include <thread>

#include "logger_subscriber.h"
#include "normalized_logger_subscriber.h"
#include "udp_subscriber.h"

#include "head_tracker.h"

#include "head_data_smoothener.h"
#include "head_data_filter.h"
#include "average_head_positioning.h"

#include "head_data_multiplier.h"
#include "head_data_normalizer.h"


#include "yal.h"

int main(int argsc, char* argsv[]) {
	__yal_init_logger();
	__yal_set_lvl_info();

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0) 
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		WSACleanup();
		return 0;
	}

	// The port for the UDP subscriber will be the first argument passed to the application
	int port = 4444;
	if (argsc >= 2) 
	{
		port = atoi(argsv[1]);
	}
	else 
	{ // Tell them they can change the UDP port
		__yal_log(__YAL_INFO, "You can change the UDP port by passing a number as the first argument to this application.\n", port);
	}

	// Tell them what port it's listening on
	__yal_log(__YAL_INFO, "UDP Subsciber will be sending data on port %i\n", port);
	head_tracker *ht = new head_tracker();
	ht->setup();

	udp_subscriber *us = new udp_subscriber(port);
	
	head_data_filter *hdf = new head_data_filter();
	ht->add_subscriber(hdf);

	head_data_smoothener *hds = new head_data_smoothener(0.2);
	hdf->add_subscriber(hds);

	average_head_positioning *ahp = new average_head_positioning(0.10);
	hds->add_subscriber(ahp);

	head_data_normalizer *hdn = new head_data_normalizer(ahp);
	hds->add_subscriber(hdn);
	//hds->add_subscriber(us);
	
	// These seem to be the best values for use with opentrack. Maybe this should be configurable?
	head_data_multiplier *hdm = new head_data_multiplier(5,5,5,70,45,25);
	hdn->add_subscriber(hdm);
	
	normalized_logger_subscriber *nls = new normalized_logger_subscriber("");
	logger_subscriber *ls = new logger_subscriber("");

	hdm->add_subscriber(us);
	
	hdm->add_subscriber(nls);
	//ahp->add_subscriber(ls);

	// logger_subscriber *ls1 = new logger_subscriber();
	// logger_subscriber *ls2 = new logger_subscriber();


	//hds->add_subscriber(ls1);
	// ahp->add_subscriber(ls2);


	while (ht)
	{
		ht->update();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	int a;
	std::cin >> a;


	return 0;
}