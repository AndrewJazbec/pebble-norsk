#include <pebble.h>
#include "autoconfig.h"




bool _klokkaer;
bool getKlokkaer(){return _klokkaer;}
void setKlokkaer(bool value){_klokkaer = value;}


void autoconfig_in_received_handler(DictionaryIterator *iter, void *context) {
	Tuple *tuple = NULL;
	
	tuple = dict_find(iter, KLOKKAER_PKEY);
	tuple ? setKlokkaer(tuple->value->int32) : false;
	
}

void autoconfig_init(){
	app_message_register_inbox_received(autoconfig_in_received_handler);
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

	
	if (persist_exists(KLOKKAER_PKEY)) {
		setKlokkaer(persist_read_bool(KLOKKAER_PKEY));
	}
	else {
		setKlokkaer(true);
	}

	
}

void autoconfig_deinit(){
	
	persist_write_bool(KLOKKAER_PKEY, _klokkaer);
}