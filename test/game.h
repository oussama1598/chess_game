#pragma once

#include <fstream>
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>
#include <catch2/catch.hpp>
#include "game/Game.h"

typedef std::vector<std::pair<std::string, std::string>> movesType;

movesType load_moves(std::string &file_path);
