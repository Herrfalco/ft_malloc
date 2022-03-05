/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 19:23:04 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/05 20:56:13 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>

#define MIN_CELL_NB			100
#define MIN_TNY_UP_LIM		128
#define MIN_SML_UP_LIM		4096

typedef enum				e_bool {
	FALSE,
	TRUE,
}							t_bool;

typedef struct				s_big_hdr {
	size_t					size;
	struct s_big_hdr		*prev;
	struct s_big_hdr		*next;
} __attribute__((packed))	t_big_hdr;

typedef struct				s_zone {
	size_t					cell_nb;
	size_t					cell_sz;
	size_t					*mem;
}							t_zone;

typedef struct				s_glob {
	t_zone					tiny;
	t_zone					small;
	t_big_hdr				*big;
	t_bool					init;
	t_bool					err;
}							t_glob;

t_glob				glob = {
	.tiny = {
		.cell_nb = MIN_CELL_NB,
		.cell_sz = MIN_TNY_UP_LIM + sizeof(size_t),
		.mem = NULL,
	},
	.small = {
		.cell_nb = MIN_CELL_NB,
		.cell_sz = MIN_SML_UP_LIM + sizeof(size_t),
		.mem = NULL,
	},
	.big = NULL,
	.init = FALSE,
	.err = FALSE,
};

void	align_zone(t_zone *zone) {
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
				if (zone_sz == page_nb * getpagesize()) {
					zone->cell_sz = cell_sz;
					zone->cell_nb = cell_nb;
					return;
				} else if (zone_sz > page_nb * getpagesize())
					break;
			}
		}
	}
}

/*
void	show_zone_param(char *name, t_zone *zone) {
	printf("%s: %lu cells of %lu octets (%lu + %lu) = %lu octets (%lu pages of %d)\n",
			name, zone->cell_nb, zone->cell_sz, sizeof(size_t), zone->cell_sz - sizeof(size_t),
			zone->cell_nb * zone->cell_sz, zone->cell_nb * zone->cell_sz / getpagesize(),
			getpagesize());
}
*/

void	init_glob(void) {
	align_zone(&glob.tiny);
	align_zone(&glob.small);
	if (glob.err)
		return;
	if ((glob.tiny.mem = mmap(NULL, glob.tiny.cell_nb * glob.tiny.cell_sz,
					PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)
		return ((void)(glob.err = TRUE));
	if ((glob.small.mem = mmap(NULL, glob.small.cell_nb * glob.small.cell_sz,
					PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)
		return ((void)(glob.err = TRUE));
	glob.init = TRUE;
}

void	*space_from_zone(t_zone *zone, uint16_t size) {
	size_t		*cell;

	for (size_t i = 0; i < zone->cell_nb; ++i) {
		cell = (size_t *)(((uint8_t *)zone->mem) + i * zone->cell_sz);
		if (!*cell) {
			*cell = size;
			return (++cell);
		}
	}
	return (NULL);
}

void 	*big_alloc(size_t size) {
	t_big_hdr		*cell = NULL;
	t_big_hdr		*ptr;

	if (size > SIZE_MAX - sizeof(t_big_hdr) || (cell = mmap(NULL, size + sizeof(t_big_hdr),
					PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)
		return (NULL);
	cell->size = size;
	if (!glob.big || glob.big > cell) {
		cell->next = glob.big;
		cell->prev = NULL;
		if (cell->next)
			cell->next->prev = cell;
		glob.big = cell;
	} else {
		for (ptr = glob.big; ptr->next && ptr->next < cell; ptr = ptr->next);
		cell->next = ptr->next;
		cell->prev = ptr;
		if (cell->next)
			cell->next->prev = cell;
		ptr->next = cell;
	}
	return (++cell);
}

void	*ft_malloc(size_t size) {
	void		*mem;

	if (!glob.init)	
		init_glob();
	if (!size || glob.err)
		return (NULL);
	if (size <= glob.tiny.cell_sz)
		if ((mem = space_from_zone(&glob.tiny, size)))
			return (mem);
	if (size <= glob.small.cell_sz)
		if ((mem = space_from_zone(&glob.small, size)))
			return (mem);
	return (big_alloc(size));
}

t_bool	free_from_zone(void *ptr, t_zone *zone) {
	if (ptr >= (void *)zone->mem && (uint8_t *)ptr
			< (uint8_t *)zone->mem + zone->cell_nb * zone->cell_sz) {
		*(size_t *)(((uint8_t *)ptr) - sizeof(size_t)) = 0;
		return (TRUE);
	}
	return (FALSE);
}

t_bool	free_ptr(void *ptr) {
	t_big_hdr	*cell = ptr;

	--cell;
	if (cell->prev)
		cell->prev->next = cell->next;
	else
		glob.big = cell->next;
	if (cell->next)
		cell->next->prev = cell->prev;
	return (!munmap(cell, cell->size + sizeof(t_big_hdr)));
}

void	ft_free(void *ptr) {
	if (!ptr)
		return;
	(void)(free_from_zone(ptr, &glob.tiny)
			|| free_from_zone(ptr, &glob.small)
			|| free_ptr(ptr));
}

char	*ptr_format(void *ptr) {
	static char		buff[sizeof(size_t) * 2 + 3];

	sprintf(buff, "%p", ptr);
	for (size_t i = 0; buff[i]; ++i)
		if (buff[i] >= 'a' && buff[i] <= 'f')
			buff[i] += 'A' - 'a';
	return (buff);
}

size_t		show_alloc_zone(t_zone *zone) {
	uint8_t		*hdr;
	uint8_t		*data;
	size_t		size;
	size_t		total = 0;

	for (size_t i = 0; i < zone->cell_nb; ++i) {
		hdr = ((uint8_t *)zone->mem) + i * zone->cell_sz;
		size = *((size_t *)hdr);
		if (size) {
			data = hdr + sizeof(size_t);
			printf("    %s - %s : %lu octets\n", ptr_format(data), ptr_format(data + size), size);
			total += size;
		}
	}
	return (total);
}

size_t		show_big_alloc(void) {
	size_t		total = 0;

	for (t_big_hdr *ptr = glob.big; ptr; ptr = ptr->next) {
		printf("    %s - %s : %lu octets\n", ptr_format(ptr + 1),
				ptr_format(((uint8_t *)(ptr + 1)) + ptr->size), ptr->size);
		total += ptr->size;
	}
	return (total);
}

void	show_alloc_mem(void) {
	size_t		total = 0;

	printf("TINY : %s\n", ptr_format(glob.tiny.mem));	
	if (glob.tiny.mem)
		total += show_alloc_zone(&glob.tiny);
	printf("SMALL : %s\n", ptr_format(glob.small.mem));	
	if (glob.small.mem)
		total += show_alloc_zone(&glob.small);
	printf("LARGE : %s\n", ptr_format(glob.big));
	if (glob.big)
		total += show_big_alloc();
	printf("Total : %lu octets\n", total);
}

int		main(void) {
	void	*mem[10000];

	for (size_t i = 0; i < 10000; ++i)
		mem[i] = ft_malloc(i);	
	for (size_t i = 0; i < 10000; ++i)
		ft_free(mem[i]);
	return (0);
}
