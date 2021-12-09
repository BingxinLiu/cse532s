// #define EXIT_SUCCESS 0
// #define EXIT_FAILURE -1
#include "threadsafe_io.hpp"

#define safe_io threadsafe_io::get_instance()

#define BUFFER_SIZE 256

#define SUCCESS 0
#define FAILURE -1

#define BEGINNING 0

#define DIRECTOR_ARGS_MIN_NUM 5
