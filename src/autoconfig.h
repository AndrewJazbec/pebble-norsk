#ifndef autoconfig_h
#define autoconfig_h

#include <pebble.h>



#define KLOKKAER_PKEY 0
bool getKlokkaer();


void autoconfig_in_received_handler(DictionaryIterator *iter, void *context); 

void autoconfig_init();

void autoconfig_deinit();

#endif