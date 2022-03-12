/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:38:48 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/10 15:56:53 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//error handling

#include "lib.h"
#include "stdio.h"

int			main(void) {
	void	*mem = malloc(52);
	void	*mem1 = malloc(10000);

	*((int *)mem) = 99999;
	*((long *)mem + 3) = 66;
	mem = realloc(mem, 5000);
	for (size_t i = 0; i < 100; ++i)
		((char *)mem1)[i] = i;
	mem1 = realloc(mem1, 50);
	free(mem);
	free(mem1);
	return (0);
}
