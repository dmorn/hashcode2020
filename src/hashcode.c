// SPDX-FileCopyrightText: 2020 84cb48
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashcode.h"

#define _MIN(x,y) ((x) <= (y)) ? (x) : (y)

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
	fflush(out);
	return 0;
}

typedef struct library_s {
	int id, books_c, signup_d, tput;
	int *books;

	// Dinamically computed fields.
	int avail_d, scan_c;
} library_t;

static void libs_free(library_t **libs, int libs_c) {
	for (int i = 0; i < libs_c; i++) {
		free(libs[i]->books);
		free(libs[i]);
	}
}

static int libs_parse(library_t **libs, int *libs_c, FILE *in) {
	for (int i = 0; i < *libs_c; i++) {
		int books_c, signup_d, tput;
		if (fscanf(in, "%d %d %d", &books_c, &signup_d, &tput) < 0)
			return -2;

		int book;
		int *books = malloc(sizeof(int)*books_c);
		for (int j = 0; j < books_c; j++) {
			if (fscanf(in, "%d", &book) < 0)
				return -3;

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

static int libs_compare_a(const void *l, const void *r) {
	library_t libl = *(library_t*)l;
	library_t libr = *(library_t*)r;

	// If the signup durations are not the same, use that
	// as comparison machanism: we want the libs that end
	// their signup process faster to be the first ones to
	// maximize parallelism.
	if (libl.signup_d != libr.signup_d)
		return libl.signup_d - libl.signup_d;

	// If the # of books of the two libs is not the same,
	// use that. We want the libs with many books to signup
	// first, to allow them to put al their books.
	if (libl.books_c != libr.books_c)
		return libl.books_c - libr.books_c;

	// Number of days required by the library to complete
	// its tasks. Sort in descending order this time!
	int rd_l = (float)libl.books_c / libl.tput;
	int rd_r = (float)libr.books_c / libr.tput;
	return rd_r - rd_l;
}

// libs_avail uses the signup duration exclusion principle, and
// expected libs to be sorted be that value in ascending order.
static int libs_avail(library_t **libs, int libs_c, int days) {
	int rv = 0;
	for (int i = 0; i < libs_c; i++) {
		int sd = libs[i]->signup_d;

		// This is the case where we overflow. We cannot
		// accept any more library (later ones are expected
		// to take even more signup time).
		if (days - sd < 0)
			return rv;

		rv++;
		days -= sd;
		libs[i]->avail_d = days;
	}
	return rv;
}

static int lib_scannable_books(library_t l, int* scores) {
	int p, e, rv;
	p = l.tput * l.avail_d; // possible
	e = _MIN(p, l.books_c); // effective
	for (int i = 0; i < e; i++) {
		if (scores[l.books[i]] > 0)
			rv++;
	}
	return rv;
}

static int lib_books_score(library_t l, int* scores) {
	int rv = 0;
	for (int i = 0; i < l.scan_c; i++) {
		rv += scores[l.books[i]];
	}
	return rv;
}

static int libs_fmt(FILE *dst, library_t **libs, int libs_c) {
	fprintf(dst, "%d\n", libs_c);
	library_t *l;
	int sb;
	for (int i = 0; i < libs_c; i++) {
		l = libs[i];
		sb = l->scan_c;
		fprintf(dst, "%d %d\n", l->id, sb);
		for (int j = 0; j < sb-1; j++) {
			fprintf(dst, "%d ", l->books[j]);
		}
		fprintf(dst, "%d\n", l->books[sb-1]);
	}
	return 0;
}

static int books_compare_a(void *thunk, const void *l, const void *r) {
	int *scores = (int*)thunk;
	int bl = *(int*)l;
	int br = *(int*)r;
	return scores[br] - scores[bl];
}

static void books_display(library_t *l) {
	fprintf(stderr, "library %d\n", l->id);
	for (int i = 0; i < l->books_c; i++) {
		fprintf(stderr, "book %d: %d\n", i, l->books[i]);
	}
}

static void libs_schedule_books(library_t **libs, int libs_c, int *scores) {
	library_t *l;
	int n;
	for (int i = libs_c-1; i >= 0; i--) {
		l = libs[i];

		// Sort books putting the most valuable ones first.
		// This also helps removing duplicates: when a book
		// is taken, its score is grounded.
		qsort_r(l->books, l->books_c, sizeof(int), scores, books_compare_a);

		// Zero score of taken books.
		l->scan_c=lib_scannable_books(*l, scores);
		for (int i = 0; i < l->scan_c; i++) {
			scores[l->books[i]] = 0;
		}
	}
}

static int libs_filter_valuable(library_t **libsf, library_t **libs, int libs_c) {
	int rv = 0;
	library_t *l;
	for (int i = 0; i < libs_c; i++) {
		l = libs[i];
		if (l->scan_c > 0) {
			libsf[rv] = l;
			rv++;
		}
	}
	return rv;
}

int hc_solve(FILE *out, FILE *in) {
	int scores_c, libs_c, days;
	fscanf(in, "%d %d %d", &scores_c, &libs_c, &days);

	int score;
	int *scores = malloc(sizeof(int)*scores_c);
	int *scores_cpy = malloc(sizeof(int)*scores_c);
	for (int i = 0; i < scores_c; i++) {
		if (fscanf(in, "%d", &score) < 0)
			return -1;
		scores[i] = score;
	}
	memcpy(scores_cpy, scores, sizeof(int)*scores_c);

	int rv;
	library_t **libs = malloc(sizeof(library_t*)*libs_c);
	if ((rv = libs_parse(libs, &libs_c, in)))
		return rv;

	qsort(libs, libs_c, sizeof(library_t*), libs_compare_a);
	int p = libs_avail(libs, libs_c, days);
	libs_schedule_books(libs, p, scores_cpy);

	library_t **libsf = malloc(sizeof(library_t*)*libs_c);
	int libsf_c = libs_filter_valuable(libsf, libs, p);


	// Format final output.
	if ((rv = libs_fmt(out, libsf, libsf_c)))
		return rv;

	free(scores);
	free(scores_cpy);
	libs_free(libs, libs_c);
	free(libsf);

	return 0;
}
