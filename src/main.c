// SPDX-FileCopyrightText: 2020 84cb48
//
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <locale.h>
#include "hashcode.h"

int main() {
	setlocale(LC_NUMERIC, "");

	int err;
	if ((err = hc_solve(stdout, stdin)) != 0) {
		fprintf(stderr, "hc_solve(): error code: %d\n", err);
		return err;
	}
	return 0;
}
