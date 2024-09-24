#include <stdio.h>
#include <stdlib.h>

int ap = 0;

void *my_malloc(size_t size) {
	void *ret = malloc(size);
	ap++;
	return ret;
}

void *my_free(void *ptr) {
	free(ptr);
	ap--;
}

#define DARY_CUSTOM_ALLOC(s) my_malloc(s)
#define DARY_CUSTOM_FREE(p) my_free(p)
#define DARY__IMPLEMENTATION_
#include "dary.h"

typedef int (*test)();

void test_print(char *s, test f) {
	printf("test: %65s: ", s);
	fflush(stdout);
	if(f()) {
		printf("SUCCESS\n");
	} else {
		printf("FAIL\n");
	}
	fflush(stdout);
}

int test1() {
	int *my_dary = make_dary(500000, sizeof(int));

	for(int i = 0; i < 500000; i++) {
		my_dary[i] = i * 2;
	}

	for(int i = 500000; i >= 0; i--) {
		my_dary[i] = i * 2;
	}

	if(dary_size(my_dary) != 500000) {
		return 0;
	}

	free_dary(my_dary);
	if(ap != 0) { return 0; }

	return 1;
}

int test2() {
	int *my_dary = make_dary(0, sizeof(int));

	for(int i = 0; i < 5000000; i++) {
		dary_grow(my_dary);
	}
	if(dary_size(my_dary) != 5000000) {
		return 0;
	}

	free_dary(my_dary);
	if(ap != 0) { return 0; }

	return 1;
}

int test3() {
	int *my_dary = make_dary(0, sizeof(int));

	for(int i = 0; i < 5000000; i++) {
		dary_end_grow(my_dary) = i * 4;
	}
	if(dary_size(my_dary) != 5000000) {
		return 0;
	}

	free_dary(my_dary);
	if(ap != 0) { return 0; }

	return 1;
}

int main() {
	test_print("simple malloc and free", test1);
	test_print("dary_grow function", test2);
	test_print("dary_end_grow macro", test3);
	return 0;
}
