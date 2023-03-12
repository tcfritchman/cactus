cmake_minimum_required(VERSION 3.24)
project(nes_emulator)

set(CMAKE_CXX_STANDARD 17)

add_executable(nes_emulator main.cpp CPU.cpp CPU.h DataBus.cpp DataBus.h)
