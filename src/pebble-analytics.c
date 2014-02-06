#include <pebble.h>

static Window *window;
static TextLayer *text_layer;


/**
 *  Analytics code
 */

enum {
    MESSAGE_TYPE,
    TRACK_SCREEN_NAME,
    TRACK_EVENT_CATEGORY,
    TRACK_EVENT_ACTION
};

enum {
    MESSAGETYPE_TRACK_SCREEN,
    MESSAGETYPE_TRACK_EVENT
};

void trackScreen(char *screenName) {
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    Tuplet value0 = TupletInteger(MESSAGE_TYPE, MESSAGETYPE_TRACK_SCREEN);
    dict_write_tuplet(iter, &value0);
    Tuplet value1 = TupletCString(TRACK_SCREEN_NAME, screenName);
    dict_write_tuplet(iter, &value1);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Tracking Stuff");
    app_message_outbox_send();
}

void trackEvent(char *category, char *action) {
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    Tuplet value0 = TupletInteger(MESSAGE_TYPE,  MESSAGETYPE_TRACK_EVENT);
    dict_write_tuplet(iter, &value0);
    Tuplet value1 = TupletCString(TRACK_EVENT_CATEGORY,  category);
    dict_write_tuplet(iter, &value1);
    Tuplet value2 = TupletCString(TRACK_EVENT_ACTION,  action);
    dict_write_tuplet(iter, &value2);
    
    app_message_outbox_send();
}

/**
 * AppMessage.
 */


char *translate_error(AppMessageResult result) {
  switch (result) {
    case APP_MSG_OK: return "APP_MSG_OK";
    case APP_MSG_SEND_TIMEOUT: return "APP_MSG_SEND_TIMEOUT";
    case APP_MSG_SEND_REJECTED: return "APP_MSG_SEND_REJECTED";
    case APP_MSG_NOT_CONNECTED: return "APP_MSG_NOT_CONNECTED";
    case APP_MSG_APP_NOT_RUNNING: return "APP_MSG_APP_NOT_RUNNING";
    case APP_MSG_INVALID_ARGS: return "APP_MSG_INVALID_ARGS";
    case APP_MSG_BUSY: return "APP_MSG_BUSY";
    case APP_MSG_BUFFER_OVERFLOW: return "APP_MSG_BUFFER_OVERFLOW";
    case APP_MSG_ALREADY_RELEASED: return "APP_MSG_ALREADY_RELEASED";
    case APP_MSG_CALLBACK_ALREADY_REGISTERED: return "APP_MSG_CALLBACK_ALREADY_REGISTERED";
    case APP_MSG_CALLBACK_NOT_REGISTERED: return "APP_MSG_CALLBACK_NOT_REGISTERED";
    case APP_MSG_OUT_OF_MEMORY: return "APP_MSG_OUT_OF_MEMORY";
    case APP_MSG_CLOSED: return "APP_MSG_CLOSED";
    case APP_MSG_INTERNAL_ERROR: return "APP_MSG_INTERNAL_ERROR";
    default: return "UNKNOWN ERROR";
  }
}

static void outbox_failed(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
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
  app_message_open(inbound_size, outbound_size);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "AppMessage set up. Inbox: %i, Outbox: %i", inbound_size, outbound_size);
}


/**
 *  Test App Stuff
 */

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Select");

  trackEvent("click", "Select");
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Up");

  trackEvent("click", "Up");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Down");

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
