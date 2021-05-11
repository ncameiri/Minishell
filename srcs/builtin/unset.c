#include "../../shell.h"

int ft_unset(char **content)
{	
	if (!content[1])
		return (-1);
	if (ft_strchr(content[1],'='))
	{
		printf("bash: unset: `%s\': not a valid identifier\n",content[1]);
		return (-1);
	}
		
	if(content[1] && ft_strlen(env_isex_elem (content[1])))
		env_rm_elem(content[1]);
	return (1);   
}