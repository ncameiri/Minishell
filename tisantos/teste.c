# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <fcntl.h>
# include <errno.h>

int main()
{
	int *teste;

	printf("%d", errno);
	printf("%s", strerror(errno));


	teste = &errno;

	*teste = 5;

	printf("\n%d", errno);
	printf("%s", strerror(errno));


}
