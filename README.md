pebble-analytics
================

Google Analytics implementation for Pebble smartwatch. Supports screen and event tracking. It uses the Google Analytics [Measurement Protocol](https://developers.google.com/analytics/devguides/collection/protocol/v1/devguide#apptracking). Your Analytics property has to be a Mobile App Propery to work with this.

## Usage

This project could be used as a demo of on the watch. Just download it, change the Analytics ID in pebble-js-app.js with your own, build and install. Whenn you press the UP, DOWN and SELECT buttons, you should see Events in your Google Analytics realtime view.

If you want to use it in your own project, take the following steps:

###On the JS side

 1. Copy contents of __pebble-ga.js__ to the beginning of __pebble-js-app.js__
 2. Initialize an analytics object like this: `var ga = new Analytics('UA-1234567-0', 'myApp', '1.0.0');`
 3. Track events or screens with `ga.trackScreen('screen name');` or `ga.trackEvent('category', 'action');`

###On the C side

 1. Copy __ga.h__ and __ga.c__ to your __src/__ folder
 2. Include ga.h in your primary app file `#include <ga.h>`
 3. Set up app messages. E.g. by calling `app_message_open(128, 128)`
 4. Track events or screens with `trackScreen("screen name");` or `trackEvent("category", "action");`
    
Keep in mind that you are responsible of handling messages. If you track to many times in a short interval on the watch, the messages could get lost. I recommend implementing some sort of message queue.
