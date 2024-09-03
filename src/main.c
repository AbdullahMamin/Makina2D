#include "engine.h"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		puts("provide only one argument: the script to run");
		return EXIT_FAILURE;
	}

	if (!runScript(argv[1]))
	{
		puts("Something went wrong in the script");
		return EXIT_FAILURE;
	}
	
	puts("All went well");
	return EXIT_SUCCESS;
}
