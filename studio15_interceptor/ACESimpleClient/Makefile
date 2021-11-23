all:
	g++ -L$(ACE_ROOT)/ace -Wall -std=c++11 -pthread -DTEMPLATE_HEADERS_INCLUDE_SOURCE -o ACESimpleClient ../interceptor.cpp ../interceptor_composer.hpp ACESimpleClient_q7.cpp -I$(ACE_ROOT) -lACE

clean:
	rm ACESimpleClient *~


