#ifndef ga_h
#define ga_h

#include <pebble.h>

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

char *translate_error(AppMessageResult result);

void trackScreen(char *screenName);
void trackEvent(char *category, char *action);

#endif