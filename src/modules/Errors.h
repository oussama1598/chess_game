#pragma once

#include <map>
#include <string>

struct Errors {
    static const std::string INVALID_DESTINATION_POSITION;
    static const std::string INVALID_SOURCE_POSITION;
    static const std::string SOURCE_SAME_AS_DESTINATION;
    static const std::string PLAYER_DOES_NOT_EXIST;
    static const std::string ILLEGAL_MOVE;
};
