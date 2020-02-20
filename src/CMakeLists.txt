# SPDX-FileCopyrightText: 2020 84cb48
#
# SPDX-License-Identifier: MIT

include(GNUInstallDirs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

add_executable(hashcode2020 main.c)