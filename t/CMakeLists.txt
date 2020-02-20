# SPDX-FileCopyrightText: 2020 84cb48
#
# SPDX-License-Identifier: MIT

add_executable(example_test example_test)
target_link_libraries(example_test cmocka)

add_test(
  NAME
    test_example
  COMMAND
    $<TARGET_FILE:example_test>
)