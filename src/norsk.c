#include "pebble.h"
#include "autoconfig.h"
#include "num2words.h"

#define BUFFER_SIZE 86

static struct CommonWordsData {
  TextLayer *label;
  char buffer[BUFFER_SIZE];
} s_data;

Window *window;

static void update_time(struct tm* t) {
  fuzzy_time_to_words(t->tm_hour, t->tm_min, s_data.buffer, BUFFER_SIZE, getKlokkaer());
  text_layer_set_text(s_data.label, s_data.buffer);
}

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  update_time(tick_time);
}

static void window_load(Window *window) {}

static void window_appear(Window *window){
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  update_time(t);
}
static void window_unload(Window *window) {}

static void in_received_handler(DictionaryIterator *iter, void *context) {
    // Let Pebble Autoconfig handle received settings
    autoconfig_in_received_handler(iter, context);

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    update_time(t);
}

static void init(void) {
  autoconfig_init();

  app_message_register_inbox_received(in_received_handler);
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
		.load = window_load,
		.unload = window_unload,
		.appear = window_appear,
	});

  window_set_background_color(window, GColorBlack);
  GFont font = fonts_get_system_font(FONT_KEY_DROID_SERIF_28_BOLD);

  Layer *root_layer = window_get_root_layer(window);
  GRect frame = layer_get_frame(root_layer);

  s_data.label = text_layer_create(GRect(0, 20, frame.size.w, frame.size.h - 20));
  text_layer_set_background_color(s_data.label, GColorBlack);
  text_layer_set_text_color(s_data.label, GColorWhite);
  text_layer_set_font(s_data.label, font);
  layer_add_child(root_layer, text_layer_get_layer(s_data.label));

  tick_timer_service_subscribe(MINUTE_UNIT, &handle_minute_tick);
  window_stack_push(window, true);
}

static void do_deinit(void) {
  window_destroy(window);
  text_layer_destroy(s_data.label);
  autoconfig_deinit();
}

int main(void) {
  init();
  app_event_loop();
  do_deinit();
}
