# SPDX-FileCopyrightText: 2020 84cb48
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(hashcode2020 LANGUAGES C)

# defines targets and sources
add_subdirectory(src)

# contains an "external" library we will link to
add_subdirectory(deps/cmocka)

# enable testing and define tests
enable_testing()
add_subdirectory(t)
