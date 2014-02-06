#include <pebble.h>

// Include the header to have access to the tracking methods.
#include <ga.h>

static Window *window;
static TextLayer *text_layer;

/**
 * AppMessage.
 */

static void outbox_failed(DictionaryIterator *iterator, AppMessageResult reason, void *context) {

  // translate_error() is a convenience function bundled in the trackin files.
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Message sending failed: %s", translate_error(reason));
}

static void outbox_sent(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Message sent");
}

static void init_app_message() {
  app_message_register_outbox_failed(outbox_failed);
  app_message_register_outbox_sent(outbox_sent);

  int inbound_size = app_message_inbox_size_maximum();
  int outbound_size = 128;

  // It is nessecary to set up app message for sending
  // tracking events to the JS app.
  app_message_open(inbound_size, outbound_size);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "AppMessage set up. Inbox: %i, Outbox: %i", inbound_size, outbound_size);
}

/**
 *  TestApp Stuff
 */

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Select");

  // Track click event with specified action.
  trackEvent("click", "Select");
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Up");

  // Track click event with specified action.
  trackEvent("click", "Up");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Down");

  // Track click event with specified action.
  trackEvent("click", "Down");
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(text_layer, "Press a button");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);

  // Track app closing.
  trackEvent("lifecycle", "app close");
}

static void init(void) {
  init_app_message();
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
