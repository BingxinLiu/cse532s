all:
	g++ -L$(ACE_ROOT)/ace -Wall -std=c++11 -pthread -DTEMPLATE_HEADERS_INCLUDE_SOURCE -o ACESimpleClient singleton_ds.hpp ACESimpleClient_q3.cpp -I$(ACE_ROOT) -lACE

clean:
	rm ACESimpleClient *~


