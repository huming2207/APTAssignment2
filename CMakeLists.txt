cmake_minimum_required(VERSION 3.7)
project(APTAssignmentTwo)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_FLAGS "-pedantic -Wall -ansi")

set(SOURCE_FILES
        addressbook.c
        addressbook.h
        addressbook_array.c
        addressbook_array.h
        addressbook_list.c
        addressbook_list.h
        commands.c
        commands.h
        helpers.c
        helpers.h)

add_executable(APTAssignmentTwo ${SOURCE_FILES})