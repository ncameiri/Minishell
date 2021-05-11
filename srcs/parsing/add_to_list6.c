#include "../../shell.h"

int	last_elem_lis(t_var_add_tlis *va, int index)
{
	va->type = 8;
	va->aux = ft_substr(mini_sh.cmd_tables[index],
			va->start, (va->i + 1) - va->start);
	ft_linkadd_back(&mini_sh.ls_start, ft_linknew(va->aux, va->type));
}

int	sep_link(int index, int *a, int *type)
{
	t_var_seplink	va;

	va.i = *a;
	va.s = 0;
	va.single_q = 0;
	va.double_q = 0;
	va.n = mini_sh.cmd_tables[index][va.i];
	sep_link_2(&va, a, index, type);
	if (va.double_q || va.single_q)
		mini_sh.error = 1;
	return (0);
}
