#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


char	*args1[3];
char	*args2[3];
int		count;

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*string;
	int		len;
	int		i;
	int		b;

	i = 0;
	b = 0;
	if (!s1)
		return (NULL);
	if (!s2 && s1)
		return ((char *)s1);
	len = strlen(s1) + strlen(s2);
	string = malloc(len + 1 * sizeof(string));
	if (string == NULL)
		return (NULL);
	while (s1[i] != '\0')
	{
		string[i] = s1[i];
		i++;
	}
	while (s2[b] != '\0')
		string[i++] = s2[b++];
	string[i] = '\0';
	return (string);
}

void set_args()
{
	args1[0] = "echo";
	args1[1] = "ola";
	args1[2] = NULL;

	args2[0] = "aqui.txt";
	args2[1] = NULL;

	count = 2;
}

int main(int argc, char **argv, char **envp)
{
	pid_t pid;

	int file_fd;

	set_args();

	int i;
	i = 0;

	while (i < count)
	{


		pid = fork();
		if (pid == -1)
			printf("error");
		/*if (pid == 0)
		{
			file_fd = open();
		}
		*/
		i++;
	}




}
