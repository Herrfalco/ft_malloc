/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 19:23:04 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/07 16:03:04 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//add output file
//error handling

#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

#define MIN_CELL_NB			100
#define MIN_TNY_UP_LIM		128
#define MIN_SML_UP_LIM		4096
#define DELIMITER			"--------------------------------------------" \
							"--------------------------------------------\n"
#define DEBUG_ENV_VAR		"M_DEBUG"
#define DEBUG_LAB			"no:minimal:layout:full"
#define DEBUG_OP			"(+) Allocate:(-) Free:(^) Increase:(v) Decrease:(&) Reallocate"
#define BUFF_SIZE			128

typedef enum				e_lab_idx {
	NO = 0,
	MINIMAL = 3,
	LAYOUT = 11,
	FULL = 18,
}							t_lab_idx;

typedef enum				e_debug {
	ALLOC = 0,
	FREE = 13,
	INC = 22,
	DEC = 35,
	REALLOC = 48,
}							t_debug;

typedef enum				e_bool {
	FALSE,
	TRUE,
}							t_bool;

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
}							t_glob;

t_glob		glob = {
	.tiny = {
		.cell_nb = MIN_CELL_NB,
		.cell_sz = MIN_TNY_UP_LIM + sizeof(t_hdr),
		.mem = NULL,
	},
	.small = {
		.cell_nb = MIN_CELL_NB,
		.cell_sz = MIN_SML_UP_LIM + sizeof(t_hdr),
		.mem = NULL,
	},
	.big = NULL,
	.init = FALSE,
	.debug = 0,
};

void		show_dump(void *ptr, size_t size, char *pad) {
	t_bool		init = TRUE;
	size_t		zero = 0;
	uint8_t		byte;


	if (glob.debug < FULL)
		return;
	for (size_t i = 0; i < size; ++i) {
		byte = ((uint8_t *)ptr)[i];
		if (init || (!zero && !(i % 24)) || (zero && byte)) {
			if (zero && byte) {
				printf(".. [%lu x 00]", zero);
				zero = 0;
			}
			if (init) {
				init = FALSE;
			} else
				printf("\n");
			printf("%s%-5lu  ", pad, i);
		}
		if (!byte) {
			zero += 1;
		} else {
			printf("%02x ", byte);
		}
	}
	if (zero)
		printf(".. [%lu x 00]", zero);
	printf("\n");
}

size_t		show_alloc_zone(char *pad, char *pad_dump, t_zone *zone) {
	t_hdr		*hdr;
	uint8_t		*data;
	size_t		size;
	size_t		total = 0;

	for (size_t i = 0; i < zone->cell_nb; ++i) {
		hdr = (t_hdr *)(((uint8_t *)zone->mem) + i * zone->cell_sz);
		size = ((t_hdr *)hdr)->size;
		if (size) {
			data = (uint8_t *)(hdr + 1);
			printf("%s%p - %p : %lu octets\n", pad, data, data + size, size);
			show_dump(data, size, pad_dump);	
			total += size;
		}
	}
	return (total);
}

size_t		show_big_alloc(char *pad, char *pad_dump) {
	size_t		total = 0;

	for (t_big_hdr *ptr = glob.big; ptr; ptr = ptr->next) {
		printf("%s%p - %p : %lu octets\n", pad, ptr + 1,
			ptr + 1 + ptr->size, ptr->size);
		show_dump(ptr + 1, ptr->size, pad_dump);	
		total += ptr->size;
	}
	return (total);
}

void		show_layout(char *pad_zone, char *pad_mem, char *pad_dump, char *pad_tot) {
	size_t		total = 0;

	printf("%sTINY  : %p\n", pad_zone, glob.tiny.mem);
	if (glob.tiny.mem)
		total += show_alloc_zone(pad_mem, pad_dump, &glob.tiny);
	printf("%sSMALL : %p\n", pad_zone, glob.small.mem);
	if (glob.small.mem)
		total += show_alloc_zone(pad_mem, pad_dump, &glob.small);
	printf("%sLARGE : %p\n", pad_zone, glob.big);
	if (glob.big)
		total += show_big_alloc(pad_mem, pad_dump);
	printf("%sTotal = %lu octets\n", pad_tot, total);
}

void		show_alloc_mem(void) {
	show_layout("", "", "", "");
}

size_t		str_len(char *str, char del) {
	size_t		len = 0;
	
	for (len = 0; str[len] && str[len] != del; ++len);
	return (len);
}

void		show_deb(t_debug deb, t_bool res, size_t s1, size_t s2, void *p1, void *p2) {
	char		buff[BUFF_SIZE];

	if (glob.debug < MINIMAL)
		return;
	printf("%-15.*s", (int)str_len(DEBUG_OP + deb, ':'), DEBUG_OP + deb);
	if (!res) {
		printf("FAILED\n");
		return;
	}
	if (deb == ALLOC || deb == FREE)
		sprintf(buff, "%ld byte(s)", s1);
	else
		sprintf(buff, "%ld -> %ld byte(s)", s1, s2);
	printf("%33s", buff);
	if (deb == REALLOC)
		sprintf(buff, "%p -> %p", p1, p2);
	else
		sprintf(buff, "%p", p1);
	printf("%40s\n", buff);
	if (glob.debug >= LAYOUT) {
		show_layout(" |  ", "    -  ", "        #", " >  ");
		printf(DELIMITER);
	}
}

void		align_zone(t_zone *zone) {
	size_t		cell_nb = zone->cell_nb;
	size_t		cell_sz = zone->cell_sz;
	size_t 		zone_sz = cell_sz * cell_nb;
	size_t		page_nb = zone_sz / getpagesize() + !!(zone_sz % getpagesize());

	for (;; ++page_nb) {
		cell_sz = zone->cell_sz;
		for (;; ++cell_sz) {
			cell_nb = zone->cell_nb;
			zone_sz = cell_nb * cell_sz;
			if (zone_sz > page_nb * getpagesize())
				break;
			for (;; ++cell_nb) {
				zone_sz = cell_nb * cell_sz;
				if (zone_sz > page_nb * getpagesize())
					break;
				if (!(zone_sz % sizeof(size_t)) && zone_sz == page_nb * getpagesize()) {
					zone->cell_sz = cell_sz;
					zone->cell_nb = cell_nb;
					return;
				}
			}
		}
	}
}

void		show_zone_param(char *name, t_zone *zone) {
	char		buff[BUFF_SIZE];

	sprintf(buff, "%s:", name);
	printf("%-7s", buff);
	sprintf(buff, "%lu cells of %lu octets (%lu + %lu)", zone->cell_nb, zone->cell_sz,
			sizeof(t_hdr), zone->cell_sz - sizeof(t_hdr));
	printf("%-40s", buff);
	sprintf(buff, "%lu octets (%lu pages of %d)", zone->cell_nb * zone->cell_sz,
			zone->cell_nb * zone->cell_sz / getpagesize(), getpagesize());
	printf("=%40s\n", buff);
}

char		sw_case(char c) {
	if (c >= 'a' && c <= 'z')
		return (c - ('a' - 'A'));
	else if (c >= 'A' && c <= 'Z')
		return (c + ('a' - 'A'));
	return (c);
}

t_lab_idx	labcmp_icase(char *val) {
	t_bool		beg = TRUE;
	t_bool		diff;
	size_t		i_sav;
	size_t		val_i;

	for (size_t i = 0;; ++i, ++val_i) {
		if (beg) {
			beg = FALSE;
			diff = FALSE;
			i_sav = i;
			val_i = 0;
		}
		if (!DEBUG_LAB[i] || DEBUG_LAB[i] == ':') {
			if (!diff)
				return(i_sav);
			if (!DEBUG_LAB[i])
				return (NO);
			beg = TRUE;
		} else if (DEBUG_LAB[i] != val[val_i] && DEBUG_LAB[i] != sw_case(val[val_i]))
			diff = TRUE;
	}
}

t_bool		init_glob(void) {
	if (getenv(DEBUG_ENV_VAR))
		glob.debug = labcmp_icase(getenv(DEBUG_ENV_VAR));
	align_zone(&glob.tiny);
	align_zone(&glob.small);
	if ((glob.tiny.mem = mmap(NULL, glob.tiny.cell_nb * glob.tiny.cell_sz,
					PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED
			|| (glob.small.mem = mmap(NULL, glob.small.cell_nb * glob.small.cell_sz,
					PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)
		return (FALSE);
	if (glob.debug > NO) {
		printf(DELIMITER);
		show_zone_param("TINY", &glob.tiny);
		show_zone_param("SMALL", &glob.small);
		printf(DELIMITER);
	}
	glob.init = TRUE;
	return (TRUE);
}

t_hdr		*space_from_zone(t_zone *zone, uint16_t size) {
	t_hdr		*cell;

	for (size_t i = 0; i < zone->cell_nb; ++i) {
		cell = (t_hdr *)(((uint8_t *)zone->mem) + i * zone->cell_sz);
		if (!cell->size) {
			cell->size = size;
			return (cell);
		}
	}
	return (NULL);
}

t_big_hdr 	*big_alloc(size_t size) {
	t_big_hdr		*cell;
	t_big_hdr		*ptr;

	if (size > SIZE_MAX - sizeof(t_big_hdr) || (cell = mmap(NULL, size + sizeof(t_big_hdr),
					PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)
		return (NULL);
	cell->size = size;
	if (!glob.big || glob.big > cell) {
		cell->next = glob.big;
		cell->prev = NULL;
		glob.big = cell;
	} else {
		for (ptr = glob.big; ptr->next && ptr->next < cell; ptr = ptr->next);
		cell->next = ptr->next;
		cell->prev = ptr;
		ptr->next = cell;
	}
	if (cell->next)
		cell->next->prev = cell;
	return (cell);
}

void		*raw_malloc(size_t size, t_bool with_hdr, t_bool *is_big) {
	t_hdr		*mem;
	t_big_hdr	*big_mem;

	if (!glob.init)
		if (!init_glob())
			return (NULL);
	if (!size)
		return (NULL);
	if (is_big)
		*is_big = FALSE;
	if (size <= glob.tiny.cell_sz - sizeof(t_hdr))
		if ((mem = space_from_zone(&glob.tiny, size)))
			return (with_hdr ? mem : mem + 1);
	if (size <= glob.small.cell_sz - sizeof(t_hdr))
		if ((mem = space_from_zone(&glob.small, size)))
			return (with_hdr ? mem : mem + 1);
	if ((big_mem = big_alloc(size))) {
		if (is_big)
			*is_big = TRUE;
		return (with_hdr ? big_mem : big_mem + 1);
	}
	return (NULL);
}

void		*ft_malloc(size_t size) {
	void	*mem = raw_malloc(size, FALSE, NULL);

	show_deb(ALLOC, !!mem, size, 0, mem, NULL);
	return (mem);
}

t_bool		in_zone(void *ptr, t_zone *zone) {
	return (ptr >= (void *)zone->mem && (uint8_t *)ptr
			< (uint8_t *)zone->mem + zone->cell_nb * zone->cell_sz);
}

size_t		free_from_zone(void *ptr, t_zone *zone) {
	t_hdr		*hdr = ((t_hdr *)ptr) - 1;
	size_t		old_size = hdr->size;

	if (!in_zone(ptr, zone))
		return (0);
	hdr->size = 0;
	return (old_size);
}

size_t		free_big(void *ptr) {
	t_big_hdr	*hdr = ((t_big_hdr *)ptr) - 1;
	size_t		old_size = hdr->size;

	if (hdr->prev)
		hdr->prev->next = hdr->next;
	else
		glob.big = hdr->next;
	if (hdr->next)
		hdr->next->prev = hdr->prev;
	return (munmap(hdr, hdr->size + sizeof(t_big_hdr)) ? 0 : old_size);
}

void		raw_free(void *ptr, t_bool debug) {
	size_t		size;

	if (!ptr)
		return;
	(void)((size = free_from_zone(ptr, &glob.tiny))
		|| (size = free_from_zone(ptr, &glob.small))
		|| (size = free_big(ptr)));
	if (debug)
		show_deb(FREE, !!size, size, 0, ptr, NULL);
}

void		ft_free(void *ptr) {
	raw_free(ptr, TRUE);
}

void		copy_n_bytes(uint8_t *dst, uint8_t *src, size_t n) {
	size_t		mod;

	if (!dst || !src)
		return;
	for (; n; n -= mod, dst += mod, src += mod) {
		if (n >= sizeof(uint64_t)) {
			*((uint64_t *)dst) = *((uint64_t *)src);
			mod = sizeof(uint64_t);
		} else if (n >= sizeof(uint32_t)) {
			*((uint32_t *)dst) = *((uint32_t *)src);
			mod = sizeof(uint32_t);
		} else if (n >= sizeof(uint16_t)) {
			*((uint16_t *)dst) = *((uint16_t *)src);
			mod = sizeof(uint16_t);
		} else if (n >= sizeof(uint8_t)) {
			*((uint8_t *)dst) = *((uint8_t *)src);
			mod = sizeof(uint8_t);
		}
	}
}

t_bool		in_place(void *ptr, size_t size, size_t	*old_size) {
	t_hdr		*hdr = ((t_hdr *)ptr) - 1;
	t_big_hdr	*big_hdr = ((t_big_hdr *)ptr) - 1;

	if (in_zone(ptr, &glob.tiny)) {
		*old_size = hdr->size;
		if (size + sizeof(t_hdr) <= glob.tiny.cell_sz) {
			hdr->size = size;
			return (TRUE);
		}
	} else if (in_zone(ptr, &glob.small)) {
		*old_size = hdr->size;
		if (size + sizeof(t_hdr) <= glob.small.cell_sz) {
			hdr->size = size;
			return (TRUE);
		}
	} else {
		*old_size = big_hdr->size;
		if (size + sizeof(t_big_hdr)
				<= ((big_hdr->size + sizeof(t_big_hdr)) / getpagesize()
					+ !!((big_hdr->size + sizeof(t_big_hdr)) % getpagesize()))
				* getpagesize()) {
			big_hdr->size = size;
			return (TRUE);
		}
	}
	return (FALSE);
}

void		*ft_realloc(void *ptr, size_t size) {
	t_hdr		*new_hdr;
	t_big_hdr	*new_big;
	t_bool		is_big;
	size_t		old_size;

	if (in_place(ptr, size, &old_size)) {
		show_deb(old_size < size ? INC : DEC, !!ptr, old_size, size,
				ptr, NULL);
		return (ptr);
	}
	new_hdr = raw_malloc(size, TRUE, &is_big);
	if (is_big) {
		new_big = (t_big_hdr *)new_hdr;
		new_big->size = size;
		copy_n_bytes((uint8_t *)(new_big + 1), ptr, old_size);
		raw_free(ptr, FALSE);
		show_deb(REALLOC, !!new_big, old_size, size, ptr, new_big + 1);
		return (new_big + 1);
	} else {
		new_hdr->size = size;
		copy_n_bytes((uint8_t *)(new_hdr + 1), ptr, old_size);
		raw_free(ptr, FALSE);
		show_deb(REALLOC, !!new_hdr, old_size, size, ptr, new_hdr + 1);
		return (new_hdr + 1);
	}
}

int			main(void) {
	void	*mem = ft_malloc(52);

/*
	mem = ft_realloc(mem, 20);
	mem = ft_realloc(mem, 228);
	mem = ft_realloc(mem, 500);
	mem = ft_realloc(mem, 10000);
	mem = ft_realloc(mem, 9000);
	mem = ft_realloc(mem, 50000);
	mem = ft_realloc(mem, 100000);
	*/
	*((int *)mem) = 99999;
	*((long *)mem + 3) = 66;
	mem = ft_realloc(mem, 5000);
	mem = ft_realloc(mem, 50);
	ft_free(mem);
	return (0);
}
