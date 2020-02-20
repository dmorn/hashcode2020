# SPDX-FileCopyrightText: 2020 84cb48
#
# SPDX-License-Identifier: MIT

add_executable(hashcode_test hashcode_test.c)

target_link_libraries(hashcode_test cmocka hashcode)

add_test(NAME hashcode_test COMMAND $<TARGET_FILE:hashcode_test>)
