#include "../../shell.h"

int ft_export()
{
    char *elem;
    char *set;
    int i;

    i=0;
    if (!ft_strchr(mini_sh.ls_start->content[1],'='))
    return -1;
    while(mini_sh.ls_start->content[1][i] && mini_sh.ls_start->content[1][i]!= '=')
        i++;    
    set = ft_substr(mini_sh.ls_start->content[1],0,i);
    elem=env_isex_elem(set);
    if(ft_strlen(elem))
       env_rm_elem(set);
    free(set);
    mini_sh.env=add_str_to_arrarr(mini_sh.env,mini_sh.ls_start->content[1]);
    return (1); 
}