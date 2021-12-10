all:
	g++ -L$(ACE_ROOT)/ace -Wall -std=c++11 -pthread -DTEMPLATE_HEADERS_INCLUDE_SOURCE -o main main.cpp director.cpp connect_service.cpp play.cpp player.cpp ../utilities/const.hpp ../utilities/threadsafe_io.cpp ../utilities/utilities.cpp -I$(ACE_ROOT) -lACE

clean:
	rm main *~


