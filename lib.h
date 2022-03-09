/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 09:50:23 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/09 10:59:52 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIB_H
#define LIB_H

#include <stdlib.h>

///////////////// malloc.c /////////////////

void		*malloc(size_t size);

///////////////// realloc.c ////////////////

void		*realloc(void *ptr, size_t size);

///////////////// free.c ///////////////////

void		free(void *ptr);

#endif //LIB_H
