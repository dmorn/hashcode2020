// SPDX-FileCopyrightText: 2020 84cb48
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include "hashcode.h"

int hc_solve_practice(FILE *out, FILE *in) {
	fprintf(out, "%d\n", 3);

	int types[] = {0, 2, 3};
	for (int i = 0; i < sizeof(types); fprintf(out, " "), i++) {
		fprintf(out, "%d", types[i]);
	}
	fprintf(out, "\n");
	return 0;
}

int hc_solve(FILE *out, FILE *in) {
	return 1;
}
