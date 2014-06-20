#pragma once

#include "string.h"
#include "pebble.h"

void fuzzy_time_to_words(uint8_t hours, uint8_t minutes, char* words, size_t length, uint8_t showPrefix);
