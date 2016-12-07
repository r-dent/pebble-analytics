#include "ga.h"
#include <pebble.h>

/**
  * Debug method
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

/**
  * Tracking methods
  */

void trackScreen(char *screenName) {
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    Tuplet value0 = TupletInteger(MESSAGE_TYPE, MESSAGETYPE_TRACK_SCREEN);
    dict_write_tuplet(iter, &value0);
    Tuplet value1 = TupletCString(TRACK_SCREEN_NAME, screenName);
    dict_write_tuplet(iter, &value1);
    
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