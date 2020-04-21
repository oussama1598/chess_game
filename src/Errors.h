#pragma once

#include <map>
#include <string>

struct Errors {
//        INVALID_SOURCE_POSITION,
//        INVALID_DESTINATION_POSITION,
//        INVALID_PLACE,
//        SOURCE_SAME_AS_DESTINATION,
//        INVALID_PLAYER_ID
    static const std::string INVALID_DESTINATION_POSITION;
    static const std::string INVALID_SOURCE_POSITION;
    static const std::string SOURCE_SAME_AS_DESTINATION;
    static const std::string PLAYER_DOES_NOT_EXIST;
    static const std::string ILLEGAL_MOVE;
};
