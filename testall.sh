#!/bin/bash

# SPDX-FileCopyrightText: 2020 84cb48
#
# SPDX-License-Identifier: MIT

in=(a_example.txt b_read_on.txt c_incunabula.txt d_tough_choices.txt e_so_many_books.txt f_libraries_of_the_world.txt)
hc="build/bin/hashcode2020"

[ ! -x ${hc} ] && { echo "did you compile the executable? I cannot find it in ${hc}"; exit 1; }

mkdir -p out
for i in "${in[@]}"; do
	echo "computing solution for $i.."
	time ${hc} < in/${i} > "out/${i%.txt}-out.txt"
done
exit 0