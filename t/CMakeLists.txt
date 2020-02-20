# SPDX-FileCopyrightText: 2020 84cb48
#
# SPDX-License-Identifier: MIT

add_executable(example_test example_test.c)
add_executable(practice_round_test practice_round_test.c)

target_link_libraries(example_test cmocka)
target_link_libraries(practice_round_test cmocka hashcode)

add_test(NAME test_example COMMAND $<TARGET_FILE:example_test>)
add_test(NAME practice_round_test COMMAND $<TARGET_FILE:practice_round_test>)
