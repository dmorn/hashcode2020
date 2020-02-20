// SPDX-FileCopyrightText: 2020 84cb48
//
// SPDX-License-Identifier: MIT

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>

#include <cmocka.h>
#include <hashcode.h>

void test_practice_round(void **state) {
	(void) state;

	// Write input to a temporary file.
	const char *input = "17 4\n2 5 6 8";
	FILE *in = tmpfile();
	if (in == NULL)
		fail_msg("unable to open tmp file");
	if (fputs(input, in) == EOF)
		fail_msg("unable to write to tmp file");

	// Input is ready. Now create another tmp file
	// that will store the expected output.
	FILE *out = tmpfile();
	if (out == NULL)
		fail_msg("unable to open output tmp file");

	// Call hashcode code.
	if (hc_solve_practice(out, in))
		fail_msg("unable to solve challenge");

	// Now out contains the solution.
	const char *output = "3\n0 2 3";

	// Read from one file then the other one.
	size_t bufs = 7;
	char buf[bufs];
	if (fread(buf, sizeof(int), bufs, out) < bufs) {
		perror("fread()");
		fail_msg("failed reading output");
	}
	if(!strcmp(buf, output))
		fail_msg("outputs are not the same!");
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_practice_round),
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
