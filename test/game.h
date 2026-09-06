#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <catch2/catch_all.hpp>
#include "game/Game.h"

typedef std::vector<std::pair<std::string, std::string>> movesType;

movesType load_moves(std::string &file_path);
