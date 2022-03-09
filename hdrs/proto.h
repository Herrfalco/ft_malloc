/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proto.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:31:48 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/09 09:53:06 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROTO_H
#define PROTO_H

///////////////// utils.c //////////////////

size_t		str_len(char *str, char del);
char		sw_case(char c);
t_lab_idx	labcmp_icase(char *val);
void		copy_n_bytes(uint8_t *dst, uint8_t *src, size_t n);
t_bool		in_zone(void *ptr, t_zone *zone);

///////////////// show.c ///////////////////

void		show_zone_param(char *name, t_zone *zone);
void		show_deb(t_debug deb, t_bool res, size_t s1, size_t s2, void *p1, void *p2);


///////////////// glob.c ///////////////////

t_bool		init_glob(void);

///////////////// malloc.c /////////////////

void		*raw_malloc(size_t size, t_bool with_hdr, t_bool *is_big);

///////////////// free.c ///////////////////

void		raw_free(void *ptr, t_bool debug);

#endif //PROTO_H
