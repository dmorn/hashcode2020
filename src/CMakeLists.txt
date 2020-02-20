# SPDX-FileCopyrightText: 2020 84cb48
#
# SPDX-License-Identifier: MIT

include(GNUInstallDirs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
	${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
	${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})

add_library(hashcode STATIC hashcode.h hashcode.c)
target_sources(hashcode
	PRIVATE
		hashcode.c
	PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/hashcode.c
	)
target_include_directories(hashcode
	PUBLIC
		${CMAKE_CURRENT_LIST_DIR}
	)

add_executable(hashcode2020 main.c)
target_link_libraries(hashcode2020 hashcode)
