/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:27:59 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/09 08:28:27 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
#define STRUCT_H

typedef struct				s_hdr {
	size_t					size;
} __attribute__((packed))	t_hdr;

typedef struct				s_big_hdr {
	size_t					size;
	struct s_big_hdr		*prev;
	struct s_big_hdr		*next;
} __attribute__((packed))	t_big_hdr;

typedef struct				s_zone {
	size_t					cell_nb;
	size_t					cell_sz;
	t_hdr					*mem;
}							t_zone;

typedef struct				s_glob {
	t_zone					tiny;
	t_zone					small;
	t_big_hdr				*big;
	t_bool					init;
	t_lab_idx				debug;
	FILE					*debug_out;
	pthread_mutex_t			mut;
}							t_glob;

#endif //STRUCT_H
