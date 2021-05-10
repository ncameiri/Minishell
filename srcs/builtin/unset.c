#include "../../shell.h"

int ft_unset()
{
   if(ft_strlen(env_isex_elem (mini_sh.ls_start->content[1])))
      env_rm_elem(mini_sh.ls_start->content[1]);
   return (1);   
}