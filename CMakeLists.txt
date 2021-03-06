# Copyright (c) 2017-present, Status Research and Development GmbH.
# All rights reserved.
#
# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree. An additional grant
# of patent rights can be found in the PATENTS file in the same directory.

cmake_minimum_required(VERSION 3.1.0)
project(libbreakpad)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

# external breakpad repo
set(BREAKPAD_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/breakpad)
include_directories ("${BREAKPAD_SOURCE_DIR}/src")

set(REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_SRC ${REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_SRC}
                                              ${CMAKE_CURRENT_SOURCE_DIR}/exceptionglobalhandler.cpp PARENT_SCOPE)

set(REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_INCLUDE_DIRS ${REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_INCLUDE_DIRS}
                                              "${BREAKPAD_SOURCE_DIR}/src"
                                              ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)

set(REACT_NATIVE_DESKTOP_MAIN_APP_EXTERNAL_MODULES_INCLUDE_DIRS ${REACT_NATIVE_DESKTOP_MAIN_APP_EXTERNAL_MODULES_INCLUDE_DIRS}
                                              ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)

# source copied from qtbreakpad.pri
set(BREAKPAD_SRCS 
    "${BREAKPAD_SOURCE_DIR}/src/common/string_conversion.cc"
    "${BREAKPAD_SOURCE_DIR}/src/common/convert_UTF.c"
    "${BREAKPAD_SOURCE_DIR}/src/common/md5.cc"
)

if (WIN32)
    set(BREAKPAD_SRCS ${BREAKPAD_SRCS}
        "${BREAKPAD_SOURCE_DIR}/src/common/windows/guid_string.cc"
        "${BREAKPAD_SOURCE_DIR}/src/client/windows/handler/exception_handler.cc"
        "${BREAKPAD_SOURCE_DIR}/src/client/windows/crash_generation/minidump_generator.cc"
        "${BREAKPAD_SOURCE_DIR}/src/client/windows/crash_generation/client_info.cc"
        "${BREAKPAD_SOURCE_DIR}/src/client/windows/crash_generation/crash_generation_client.cc"
    )
endif(WIN32)

if (UNIX AND NOT APPLE)
    set(BREAKPAD_SRCS ${BREAKPAD_SRCS}
        "${BREAKPAD_SOURCE_DIR}/src/client/minidump_file_writer.cc"
        "${BREAKPAD_SOURCE_DIR}/src/client/linux/log/log.cc"
        "${BREAKPAD_SOURCE_DIR}/src/client/linux/handler/exception_handler.cc"
        "${BREAKPAD_SOURCE_DIR}/src/client/linux/handler/minidump_descriptor.cc"
        "${BREAKPAD_SOURCE_DIR}/src/common/linux/guid_creator.cc"
        "${BREAKPAD_SOURCE_DIR}/src/client/linux/dump_writer_common/thread_info.cc"
        "${BREAKPAD_SOURCE_DIR}/src/client/linux/dump_writer_common/ucontext_reader.cc"
        "${BREAKPAD_SOURCE_DIR}/src/client/linux/minidump_writer/linux_dumper.cc"
        "${BREAKPAD_SOURCE_DIR}/src/client/linux/minidump_writer/minidump_writer.cc"
        "${BREAKPAD_SOURCE_DIR}/src/client/linux/minidump_writer/linux_ptrace_dumper.cc"
        "${BREAKPAD_SOURCE_DIR}/src/client/linux/microdump_writer/microdump_writer.cc"
        "${BREAKPAD_SOURCE_DIR}/src/common/linux/file_id.cc"
        "${BREAKPAD_SOURCE_DIR}/src/common/linux/elfutils.cc"
        "${BREAKPAD_SOURCE_DIR}/src/common/linux/linux_libc_support.cc"
        "${BREAKPAD_SOURCE_DIR}/src/common/linux/memory_mapped_file.cc"
        "${BREAKPAD_SOURCE_DIR}/src/common/linux/safe_readlink.cc"
        "${BREAKPAD_SOURCE_DIR}/src/client/linux/crash_generation/crash_generation_client.cc"
    )
endif(UNIX AND NOT APPLE)

if (APPLE)
    set(BREAKPAD_SRCS ${BREAKPAD_SRCS}
        "${BREAKPAD_SOURCE_DIR}/src/client/minidump_file_writer.cc"
        "${BREAKPAD_SOURCE_DIR}/src/client/mac/crash_generation/crash_generation_client.cc"
        "${BREAKPAD_SOURCE_DIR}/src/client/mac/handler/exception_handler.cc"
        "${BREAKPAD_SOURCE_DIR}/src/client/mac/handler/minidump_generator.cc"
        "${BREAKPAD_SOURCE_DIR}/src/client/mac/handler/breakpad_nlist_64.cc"
        "${BREAKPAD_SOURCE_DIR}/src/client/mac/handler/dynamic_images.cc"
        "${BREAKPAD_SOURCE_DIR}/src/client/mac/handler/protected_memory_allocator.cc"
        "${BREAKPAD_SOURCE_DIR}/src/common/mac/bootstrap_compat.cc"
        "${BREAKPAD_SOURCE_DIR}/src/common/mac/file_id.cc"
        "${BREAKPAD_SOURCE_DIR}/src/common/mac/macho_id.cc"
        "${BREAKPAD_SOURCE_DIR}/src/common/mac/macho_reader.cc"
        "${BREAKPAD_SOURCE_DIR}/src/common/mac/macho_utilities.cc"
        "${BREAKPAD_SOURCE_DIR}/src/common/mac/macho_walker.cc"
        "${BREAKPAD_SOURCE_DIR}/src/common/mac/string_utilities.cc"
        "${BREAKPAD_SOURCE_DIR}/src/common/mac/MachIPC.mm"
    )
endif(APPLE)

message(STATUS "BREAKPAD_SRCS: ${BREAKPAD_SRCS}")

# target library
add_library (libbreakpad ${BREAKPAD_SRCS})

# configure install
install (TARGETS libbreakpad DESTINATION libbreakpad/Debug CONFIGURATIONS Debug)
install (TARGETS libbreakpad DESTINATION libbreakpad/Release CONFIGURATIONS Release)

set(REACT_NATIVE_DESKTOP_MAIN_APP_EXTERNAL_PROJECT_DEPS ${REACT_NATIVE_DESKTOP_MAIN_APP_EXTERNAL_PROJECT_DEPS} libbreakpad PARENT_SCOPE)

