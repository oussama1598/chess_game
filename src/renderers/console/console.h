#pragma once

#include <iostream>
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>

#include "game/Game.h"

class ConsoleRenderer {
public:
    static void clear();

    static void render(Game &game);
};
