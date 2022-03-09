/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:38:48 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/09 08:40:28 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//why rlimit ?
//error handling

#include "header.h"

int			main(void) {
	void	*mem = ft_malloc(52);
	void	*mem1 = ft_malloc(10000);

	*((int *)mem) = 99999;
	*((long *)mem + 3) = 66;
	mem = ft_realloc(mem, 5000);
	for (size_t i = 0; i < 100; ++i)
		((char *)mem1)[i] = i;
	mem1 = ft_realloc(mem1, 50);
	ft_free(mem);
	ft_free(mem1);
	return (0);
}
