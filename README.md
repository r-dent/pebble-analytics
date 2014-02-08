pebble-analytics
================

Google Analytics implementation for Pebble smartwatch. Supports screen and event tracking. It uses the Google Analytics [Measurement Protocol](https://developers.google.com/analytics/devguides/collection/protocol/v1/devguide#apptracking). Your Analytics property has to be a Mobile App Propery to work with this.

## Usage

This project could be used as a demo of on the watch. Just download it, change the Analytics ID in pebble-js-app.js with your own, build and install. Whenn you press the UP, DOWN and SELECT buttons, you should see Events in your Google Analytics realtime view.

If you want to use it in your own project, take the following steps:

###App Configuration

If you are using tracking just in your JS app, there is no setup needed. You can read on in section __a)__.

If you want to track screens direct on the watch, you have to set up *App Message* for the communication between Watch and JS. If you already did that, you need to set the nessecary keys by configuring them in your __appinfo.json__ (see [example](https://github.com/r-dent/pebble-analytics/blob/master/appinfo.json)).

    "appKeys" : {
      "message_type" : 0,
      "track_screen_name" : 1,
      "track_event_category" : 2,
      "track_event_action" : 3
    }
    
The `trackScreen()` and `trackEvent()` Functions in your C project will send messages with these keys to the JS app. There you will need to handle them and trigger the JS Functions as described in __a) 1.__

### a) On the JS side

 1. Copy contents of __pebble-ga.js__ to the beginning of __pebble-js-app.js__
 2. Initialize an analytics object like this: `var ga = new Analytics('UA-1234567-0', 'myApp', '1.0.0');`
 3. Track events or screens with `ga.trackScreen('screen name');` or `ga.trackEvent('category', 'action');`
 4. (optional) If you use tracking on the Watch (C), then you need to handle the messages sent from there. See the [pebble-js-app.js](https://github.com/r-dent/pebble-analytics/blob/master/src/js/pebble-js-app.js) and search for _appmessage_ to see how to do that.

### b) On the C side

 1. Copy __ga.h__ and __ga.c__ to your __src/__ folder
 2. Include ga.h in your primary app file `#include <ga.h>`
 3. Set up app messages. E.g. by calling `app_message_open(128, 128)`
 4. Track events or screens with `trackScreen("screen name");` or `trackEvent("category", "action");`
    
Keep in mind that you are responsible of handling messages. If you track to many times in a short interval on the watch, the messages could get lost. I recommend implementing some sort of message queue.
