/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proto.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:31:48 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/29 20:49:40 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROTO_H
#define PROTO_H

///////////////// str.c ////////////////////

size_t				str_len(char *str, char del);
void				str_push(char *dst, char *src, t_just just, ssize_t width, ssize_t prec);
void				str_print(char *str, t_just just, ssize_t width, ssize_t prec);
void				str_push_size(char *dst, size_t nb, t_just just, ssize_t width);
void				str_print_size(size_t nb, t_just just, ssize_t width);
void				str_push_hex(char *dst, size_t nb, t_just just, ssize_t width, ssize_t prec);
void				str_print_hex(size_t nb, t_just just, ssize_t width, ssize_t prec);

///////////////// utils.c //////////////////

char				sw_case(char c);
t_lab_idx			labcmp_icase(char *val, char *labs);
void				copy_n_bytes(uint8_t *dst, uint8_t *src, size_t n);
size_t				round_nb(size_t nb, size_t delta);

///////////////// loc.c //////////////////

t_loc				wich_loc(void *ptr, t_frame_hdr **frame);

///////////////// show.c ///////////////////

void				show_deb(t_debug deb, t_bool res, size_t s1, size_t s2, void *p1, void *p2);

///////////////// malloc.c /////////////////

void				*raw_malloc(size_t size, t_bool with_hdr, t_bool *is_big);

///////////////// free.c ///////////////////

size_t				raw_free(void *ptr);

#endif //PROTO_H
