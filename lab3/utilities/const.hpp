// #define EXIT_SUCCESS 0
// #define EXIT_FAILURE -1
#include <string>

#include "threadsafe_io.hpp"

#define safe_io threadsafe_io::get_instance()

#define BUFFER_SIZE 256

#define SUCCESS 0
#define FAILURE -1

#define BEGINNING 0

#define DIRECTOR_ARGS_MIN_NUM 5

#define NONE 0

#ifndef DELIMIT
#define DELIMIT
const std::string delimit_token = "[scene]";
const size_t delimit_token_offset = delimit_token.size();
#endif

#define DEBUG 0
#define SLOW 1

#define ID_COMMAND "[director_id]"
#define START_COMMAND "start"
#define STOP_COMMAND "stop"
#define QUIT_COMMAND "quit"

#define REG_PLAY_COMMAND "[PLAY]"
#define STARTED_CONFIRM "[STARTED]"
#define STOPPED_CONFIRM "[STOPPED]"
#define QUIT_CONFIRM "[QUITED]"