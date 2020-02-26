// SPDX-FileCopyrightText: 2020 84cb48
//
// SPDX-License-Identifier: MIT

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>

#include <cmocka.h>
#include <hashcode.h>

void test_a_example(void **state) {
	// Write input to a temporary file.
	const char *input = "6 2 7\n1 2 3 6 5 4\n5 2 2\n0 1 2 3 4\n4 3 1\n0 2 3 5\n";

	FILE *in = tmpfile();
	if (in == NULL)
		fail_msg("unable to open tmp file");
	if (fputs(input, in) == EOF)
		fail_msg("unable to write to tmp file");
	rewind(in);

	// Input is ready. Now create another tmp file
	// that will store the expected output.
	FILE *out = tmpfile();
	if (out == NULL)
		fail_msg("unable to open output tmp file");

	// Call hashcode code.
	if (hc_solve(out, in))
		fail_msg("unable to solve challenge");
	rewind(out);

	// Now out contains the solution.
	const char *expout = "2\n0 5\n3 4 2 1 0\n1 1\n5\n";

	size_t n = strlen(expout);
	char *buf = malloc(n);
	if (fread(buf, sizeof(char), n, out) < n) {
		fail_msg("failed reading output file");
	}
	if(memcmp(buf, expout, n) != 0)
		fail_msg("unexpected output! wanted\n\"%s\"\nfound\n\"%s\"", expout, buf);
	free(buf);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_a_example),
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
