/* license */
/*
  Copyright (C) 2024 Eduardo Beloni Mailan

  Permission to use, copy, modify, and/or distribute this software for any
  purpose with or without fee is hereby granted.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
  REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
  AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
  INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
  LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
  OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
  PERFORMANCE OF THIS SOFTWARE.
*/

/* translation: just drop this anywhere */

/* library information */
/*
  this is a header only library for dynamic arrays in C.
  tries its best not to use macros, but includes two for convenience's sake

  MEANT FOR PROTOTYPES ONLY (aborts on failed malloc and realoc).

  the user can provide their own custom alloc, free, abort and choose not
  to use the stdlib implementations.
*/

#ifndef DARY__HEADER_
#define DARY__HEADER_

#ifndef DARY_CUSTOM_ALLOC
#ifndef DARY_DONT_INCLUDE_STDLIB
#include <stdlib.h>
#endif /* DARY_DONT_INCLUDE_STDLIB */
#define DARY_CUSTOM_ALLOC malloc
#endif /* DARY_CUSTOM_ALLOC */

#ifndef DARY_CUSTOM_REALLOC
#ifndef DARY_DONT_INCLUDE_STDLIB
#include <stdlib.h>
#endif /* DARY_DONT_INCLUDE_STDLIB */
#define DARY_CUSTOM_REALLOC realloc
#endif /* DARY_CUSTOM_REALLOC */

#ifndef DARY_CUSTOM_FREE
#ifndef DARY_DONT_INCLUDE_STDLIB
#include <stdlib.h>
#endif /* DARY_DONT_INCLUDE_STDLIB */
#define DARY_CUSTOM_FREE free
#endif /* DARY_CUSTOM_FREE */

#ifndef DARY_CUSTOM_ABORT
#ifndef DARY_DONT_INCLUDE_STDLIB
#include <stdlib.h>
#endif /* DARY_DONT_INCLUDE_STDLIB */
#define DARY_CUSTOM_ABORT abort
#endif /* DARY_CUSTOM_ABORT */

#ifndef DARY_CUSTOM_SIZE_T
#ifndef DARY_DONT_INCLUDE_STDDEF
#include <stddef.h>
#endif /* DARY_DONT_INCLUDE_STDDEF */
#define DARY_SIZE_T size_t
#else /* DARY_CUSTOM_DARY_SIZE_T */
#define DARY_SIZE_T DARY_CUSTOM_DARY_SIZE_T
#endif  /* DARY_CUSTOM_DARY_SIZE_T */

void *make_dary(DARY_SIZE_T elements, DARY_SIZE_T size_of_element);
DARY_SIZE_T dary_size(void *dary);
#define dary_grow(d) f_dary_grow((void **)(&d))
void *f_dary_grow(void **dary);
void *DARY_CUSTOM_FREE_dary(void *dary);
#define dary_end_grow(dary) \
	((typeof(dary))(dary_grow(dary)))[dary_size(dary) - 1]

#ifdef DARY__IMPLEMENTATION_

void *make_dary(DARY_SIZE_T elements, DARY_SIZE_T size_of_element) {
	DARY_SIZE_T *dary = (DARY_SIZE_T *)
		DARY_CUSTOM_ALLOC(
			elements * size_of_element + sizeof(DARY_SIZE_T) * 2);
	if(dary == NULL) {
		DARY_CUSTOM_ABORT();
	}
	dary[0] = elements;
	dary[1] = size_of_element;
	return (void *)dary + sizeof(DARY_SIZE_T) * 2;
}

DARY_SIZE_T dary_size(void *dary) {
	return ((DARY_SIZE_T *)(dary - sizeof(DARY_SIZE_T) * 2))[0];
}

void *f_dary_grow(void **dary) {
	void *arr = ((DARY_SIZE_T *)(*dary)) - 2;
	DARY_SIZE_T *elements        = &(((DARY_SIZE_T *)arr)[0]);
	DARY_SIZE_T *size_of_element = &(((DARY_SIZE_T *)arr)[1]);

	*elements += 1;
	DARY_SIZE_T new_size =
		(*elements) * (*size_of_element) + sizeof(DARY_SIZE_T) * 2;
	void *narr = DARY_CUSTOM_REALLOC(arr, new_size);
	if(narr == NULL) {
		DARY_CUSTOM_ABORT();
	}
	*dary = narr + sizeof(DARY_SIZE_T) * 2;
	return *dary;
}
void *free_dary(void *dary) {
	if(dary == NULL) {
		DARY_CUSTOM_ABORT();
	}
	DARY_CUSTOM_FREE(dary - sizeof(DARY_SIZE_T) * 2);
}

#undef DARY_DARY_SIZE_T

#endif /* DARY__IMPLEMENTATION_ */
#endif /* DARY__HEADER_ */
