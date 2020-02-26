// SPDX-FileCopyrightText: 2020 84cb48
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hashcode.h"

#define MIN(x,y) ((x) <= (y)) ? (x) : (y)

typedef struct library_s {
	int id, books_c, signup_d, tput;
	int *books;

	// Dinamically computed fields.
	int avail_d, scan_c;
} library_t;

static void libs_free(library_t *libs, int libs_c) {
	for (int i = 0; i < libs_c; i++) {
		free(libs[i].books);
	}
	free(libs);
}

static int libs_parse(library_t *libs, int libs_c, FILE *in) {
	library_t *lib;
	for (int i = 0; i < libs_c; i++) {
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
		lib = &libs[i];
		lib->id = i;
		lib->books_c = books_c;
		lib->signup_d = signup_d;
		lib->tput = tput;
		lib->books = books;
		lib->scan_c = books_c;
	}
	return 0;
}

static int lib_books_score(library_t l, int *scores) {
	int rv = 0;
	for (int i = 0; i < l.scan_c; i++) {
		rv += scores[l.books[i]];
	}
	return rv;
}

static int libs_cmp_b(void *thunk, const void *l, const void *r) {
	library_t libl = *(library_t*)l;
	library_t libr = *(library_t*)r;
	int *scores = (int*)thunk;

	int scorel = round((double)lib_books_score(libl, scores)/libl.signup_d);
	int scorer = round((double)lib_books_score(libr, scores)/libr.signup_d);
	return scorer - scorel;
}

static void libs_sort_cmp_b(library_t *libs, int libs_c, int *scores) {
	qsort_r(libs, libs_c, sizeof(library_t), scores, libs_cmp_b);
}

// libs_avail uses the signup duration exclusion principle, and
// expected libs to be sorted be that value in ascending order.
static int libs_avail(library_t *libs, int libs_c, int days) {
	int rv = 0;
	for (int i = 0; i < libs_c; i++) {
		int sd = libs[i].signup_d;

		// This is the case where we overflow. We cannot
		// accept any more library (later ones are expected
		// to take even more signup time).
		if (days - sd < 0)
			return rv;

		rv++;
		days -= sd;
		(&libs[i])->avail_d = days;
	}
	return rv;
}

static int lib_max_scan_c(library_t l, int* scores) {
	int p, e, rv;
	p = l.tput * l.avail_d; // possible
	e = MIN(p, l.books_c);  // effective
	for (int i = 0; i < e; i++) {
		if (scores[l.books[i]] > 0)
			rv++;
	}
	return rv;
}

static int books_cmp_score(void *thunk, const void *l, const void *r) {
	int *scores = (int*)thunk;
	int bl = *(int*)l;
	int br = *(int*)r;
	return scores[br] - scores[bl];
}

static void libs_schedule_books(library_t *libs, int libs_c, int *scores) {
	library_t *l;
	int n;
	for (int i = libs_c-1; i >= 0; i--) {
		l = &libs[i];

		// Sort books putting the most valuable ones first.
		// This also helps removing duplicates: when a book
		// is taken, its score is grounded.
		qsort_r(l->books, l->books_c, sizeof(int), scores, books_cmp_score);

		l->scan_c = lib_max_scan_c(*l, scores);

		// Zero score of taken books.
		for (int i = 0; i < l->scan_c; i++) {
			scores[l->books[i]] = 0;
		}
	}
}

static int libs_books_score(library_t *libs, int libs_c, int *scores) {
	int rv = 0;
	for (int i = 0; i < libs_c; i++) {
		if (libs[i].scan_c > 0)
			rv += lib_books_score(libs[i], scores);
	}
	return rv;
}

static int libs_count_valuable(library_t *libs, int libs_c) {
	int rv = 0;
	for (int i = 0; i < libs_c; i++) {
		if (libs[i].scan_c > 0)
			rv++;
	}
	return rv;
}

static int libs_fmt(FILE *dst, library_t *libs, int libs_c) {
	fprintf(dst, "%d\n", libs_count_valuable(libs, libs_c));
	library_t l;
	int sb;
	for (int i = 0; i < libs_c; i++) {
		l = libs[i];
		sb = l.scan_c;
		if (sb <= 0)
			continue;

		fprintf(dst, "%d %d\n", l.id, sb);
		for (int j = 0; j < sb-1; j++) {
			fprintf(dst, "%d ", l.books[j]);
		}
		fprintf(dst, "%d\n", l.books[sb-1]);
	}
	return 0;
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
	library_t *libs = malloc(sizeof(library_t)*libs_c);
	if ((rv = libs_parse(libs, libs_c, in)))
		return rv;

	libs_sort_cmp_b(libs, libs_c, scores);

	int p = libs_avail(libs, libs_c, days);
	libs_schedule_books(libs, p, scores_cpy);

	fprintf(stderr, "--- detected %d(%d) valuable libraries out of %d\n", libs_count_valuable(libs, p), p, libs_c);
	fprintf(stderr, "--- total score: %'d\n", libs_books_score(libs, p, scores));

	// Format final output.
	if ((rv = libs_fmt(out, libs, libs_c)))
		return rv;

	free(scores);
	free(scores_cpy);
	libs_free(libs, libs_c);

	return 0;
}
