#pragma once

#include <iostream>
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>

#include "game/Game.h"

class ConsoleRenderer {
private:
    static std::map<char, std::map<std::string, std::string>> pieces_graphics_;
public:
    static void clear();

    static void render(Game &game);
};
