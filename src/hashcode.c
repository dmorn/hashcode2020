// SPDX-FileCopyrightText: 2020 84cb48
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include "hashcode.h"

// This is just a mocked implementetion that does
// not really solve the problem. Used to check if
// the testsuite behaves as expected.
int hc_solve_practice(FILE *out, FILE *in) {
	fprintf(out, "%d\n", 3);

	char types[3] = {0, 2, 3};
	int n = sizeof(types);
	for (int i = 0; i < n-1; i++) {
		fprintf(out, "%d ", types[i]);
	}
	fprintf(out, "%d\n", types[n-1]);
	return 0;
}

typedef struct library_s {
	int id, books_c, signup_d, tput;
	int *books;
} library_t;

void free_libs(library_t **libs, int libs_c) {
	for (int i = 0; i < libs_c; i++) {
		free(libs[i]->books);
		free(libs[i]);
	}
}

int parse_libs(library_t **libs, int *libs_c, FILE *in) {
	for (int i = 0; i < *libs_c; i++) {
		int books_c, signup_d, tput;
		if (fscanf(in, "%d %d %d", &books_c, &signup_d, &tput) < 0) {
			return -2;
		}
		int book;
		int *books = malloc(sizeof(int)*books_c);
		for (int j = 0; j < books_c; j++) {
			if (fscanf(in, "%d", &book) < 0) {
				return -3;
			}
			books[j] = book;
		}
		library_t *lib = malloc(sizeof(library_t));
		lib->id = i;
		lib->books_c = books_c;
		lib->signup_d = signup_d;
		lib->tput = tput;
		lib->books = books;

		libs[i] = lib;
	}
	return 0;
}

int hc_solve(FILE *out, FILE *in) {
	int scores_c, libs_c, days;
	fscanf(in, "%d %d %d", &scores_c, &libs_c, &days);

	int score;
	int *scores = malloc(sizeof(int)*scores_c);
	for (int i = 0; i < scores_c; i++) {
		if (fscanf(in, "%d", &score) < 0) {
			return -1;
		}
	}

	int rv;
	library_t **libs = malloc(sizeof(library_t)*libs_c);
	if ((rv = parse_libs(libs, &libs_c, in)))
		return rv;

	free(scores);
	free_libs(libs, libs_c);
	return 1;
}
