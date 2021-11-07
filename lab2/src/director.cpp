

#include "director.hpp"

Director::Director(string script_file, unsigned int min_player_number)
{
    // The constructor should open the script file in an input file stream and read through it (if the script file cannot be opened the constructor should throw an exception). Whenever a new scene appears in the script (indicated by a line that begins with [scene] followed by the scene's title), the constructor should store scene's title (i.e., the rest of the line after the [scene] token) into a C++ style string and push that string back into a container member variable that holds the titles of the scenes (each of which will be printed out by the Play class object whenever the scene changes.

    // Lines of the script file that do not start with a [scene] token should be interpreted as containing names of configuration files for the various fragments of the script. The constructor should attempt to open each configuration file, and for each configuration file that it can open should (1) count how many part definition lines each configuration file contains, (2) keep track of the maximum sum of the numbers of part configuration lines that appear in any two consecutive configuration files, and (3) when it reaches a configuration file line that immediately follows another configuration file line (i.e., does not immediately follow a new scene line that begins with the [scene] token) should push an empty string into container of scene titles.

    // After it finishes reading the script file, the constructor should dynamically (i.e, using the new operator) allocate a Play object (passing a reference to the container of scene title strings into the Play constructor), and store a shared_ptr (or other appropriate C++ smart pointer) to that Play object in a member variable so that when the Director is destroyed so is the Play.

    // The Director class constructor should then take the maximum of the passed unsigned integer value and the maximum number of part lines in two consecutive script fragments, and should dynamically allocate (i.e, again using the new operator) that many Player objects (passing each one a reference to the Play object, and should push back a shared_ptr to each one into a container member variable of the Director class.
}

void 
Director::cue()
{
    // a cue method that can repeatedly hand off the name of a character and the name of a part definition file for that character, and a scene fragment number, to a Player, and with the information they contain run the Player's read and act methods to perform that part within the play.

    // For either of these variations (or for any alternative approach you may come up with) a termination protocol is needed so that after the script file has been completed, the Player threads all terminate and the Director and Player objects and all other resources of the program are freed after which the entire program ends. For the approach combining the HS/HA and Active Object patterns, for example, it may be straightforward to use a special termination ACT that is passed to each Player object's queue, which would cause the Player's thread to exit, and the Director could then join with each of those threads (e.g., by calling each Player's exit method in which the join would occur as in lab 1. For the L/F approach, it may be appropriate to modify the Director class so that the leader can detect when there are no more parts to be played, it can simply end its own thread, let the next leader be elected which then also will terminate, etc.

}