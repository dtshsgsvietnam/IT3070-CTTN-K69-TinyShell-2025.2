# Makefile – TinyShell (MinGW / Windows)
# Chạy: mingw32-make   hoặc   make

CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g -Iinclude
LDFLAGS  = -lkernel32
TARGET   = bin/myShell.exe

# Danh sach source duoc build
SRCS = src/main.cpp src/son_commands.cpp src/huy_commands.cpp src/manh_commands.cpp src/cuong_commands.cpp

all: $(TARGET)

$(TARGET): $(SRCS) | bin
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

bin:
	if not exist bin mkdir bin

clean:
	if exist "bin\myShell.exe" del /Q "bin\myShell.exe"

.PHONY: all clean
