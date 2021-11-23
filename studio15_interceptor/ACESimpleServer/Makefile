all:
	g++ -L$(ACE_ROOT)/ace -Wall -std=c++11 -pthread -DTEMPLATE_HEADERS_INCLUDE_SOURCE -o ACESimpleServer ../interceptor.cpp ../interceptor_composer.hpp ACESimpleServer_q7.cpp -I$(ACE_ROOT) -lACE

clean:
	rm ACESimpleServer *~


