#include <stdio.h>

int main(void)
{
	int prev = 1;
	for (int i = 0; i < 10; ++i)
	{
		printf("%d ", prev);
		prev = prev * 3;
	}
	printf("\n");
}
