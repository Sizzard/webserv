#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/epoll.h>
#include <vector>
#include <csignal>
#include <arpa/inet.h>
#include <cstdlib>
#include <stdio.h>
#include <fstream>

const std::string red("\033[1;31m");
const std::string green("\033[1;32m");
const std::string yellow("\033[1;33m");
const std::string blue("\033[1;34m");
const std::string cyan("\033[1;36m");
const std::string magenta("\033[1;35m");
const std::string reset("\033[0m");

#define SUCCES 0
#define FAILURE 1