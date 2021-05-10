#include "../../shell.h"

int ft_unset(char **content)
{
   if(ft_strlen(env_isex_elem (content[1])))
      env_rm_elem(content[1]);
   return (1);   
}