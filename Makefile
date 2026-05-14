# Makefile – TinyShell (MinGW / Windows)
# Chạy: mingw32-make   hoặc   make

CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g
LDFLAGS  = -lkernel32
TARGET   = myShell.exe

# Thêm file .cpp của Mạnh và Cường vào đây khi họ gửi code
SRCS = myshell.cpp son_commands.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	del /Q $(TARGET) 2>nul || rm -f $(TARGET)

.PHONY: all clean