/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 09:50:23 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/13 14:46:59 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIB_H
#define LIB_H

#include <stddef.h>

///////////////// malloc.c /////////////////

void		*malloc(size_t size);

///////////////// realloc.c ////////////////

void		*realloc(void *ptr, size_t size);

///////////////// free.c ///////////////////

void		free(void *ptr);

///////////////// show.c ///////////////////

void		show_alloc_mem(void);

#endif //LIB_H
