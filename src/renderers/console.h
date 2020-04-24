#pragma once

#include <iostream>
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>

#include "../modules/Game.h"

class ConsoleRenderer {
public:
    static void clear();

    static void render(Game &game);
};
