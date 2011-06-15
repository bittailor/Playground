#include <iostream>


int playground(int argc, const char* argv[])
{
	return 0;
}


int main(int argc, const char* argv[])
{
	std::string executable(argv[0]);
	executable = executable.substr(executable.find_last_of("/\\")+1);
	std::cout << "<"  << executable << ">" << std::endl;
	playground(argc,argv);
	std::cout << "</" << executable << ">" << std::endl;
	return 0;
}
