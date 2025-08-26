#include "pupu.h"
#include <stdio.h>

int main(void)
{
	char *test = "Hello world!";
	// printf("find_first_of(o): %s\n", find_first_of(test, "\0"));
	// printf("find_last_of(o): %s\n", find_last_of(test, "\0"));
	// printf("find_first_not_of(H!): %s\n", find_first_not_of(test, "H!"));
	printf("find_last_not_of(H!): %s\n", find_last_not_of(test, "H!"));
	return 0;
}