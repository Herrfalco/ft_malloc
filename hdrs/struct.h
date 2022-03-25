/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:27:59 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/25 19:45:30 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
#define STRUCT_H

typedef struct				s_cell_hdr {
	size_t					size;
} __attribute__((packed))	t_cell_hdr;

typedef struct				s_frame_hdr {
	struct s_frame_hdr		*prev;
	struct s_frame_hdr		*next;
	size_t					infill;
} __attribute__((packed))	t_frame_hdr;

typedef struct				s_zone {
	size_t					cell_nb;
	size_t					cell_sz;
	t_frame_hdr				*frame;
}							t_zone;

typedef struct				s_big_hdr {
	size_t					size;
	size_t					capacity;
	struct s_big_hdr		*prev;
	struct s_big_hdr		*next;
} __attribute__((packed))	t_big_hdr;

typedef struct				s_glob {
	t_zone					tiny;
	t_zone					small;
	t_big_hdr				*big;
	t_lab_idx				debug;
	t_lab_idx				show;
	pthread_mutex_t			mut;
	t_bool					fline;
}							t_glob;

#endif //STRUCT_H
