// SPDX-FileCopyrightText: 2020 84cb48
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include "hashcode.h"

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

int hc_solve(FILE *out, FILE *in) {
	return 1;
}
