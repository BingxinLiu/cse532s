// string::size
#include <iostream>
#include <string>

int main ()
{
  std::string str ("    1");
  std::cout << "The size of str is " << str.size() << " bytes.\n";
  std::cout << "The size of str is " << str.find_first_not_of(" ", 0, str.size()) << " bytes.\n";
  return 0;
}