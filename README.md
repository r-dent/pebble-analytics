pebble-analytics
================

Google Analytics implementation for Pebble smartwatch. Supports screen and event tracking. It uses the Google Analytics [Measurement Protocol](https://developers.google.com/analytics/devguides/collection/protocol/v1/devguide#apptracking). Your Analytics property has to be a Mobile App Propery to work with this.

## Usage

###On the JS side

First copy contents of __pebble-ga.js__ to the beginning of __pebble-js-app.js__

Then initialize an analytics object like this:

    var ga = new Analytics('UA-1234567-0', 'myApp', '1.0.0');

Track events or screen with like this:

    ga.trackScreen('screen name');
    ga.trackEvent('category', 'action');

###On the C side

Copy ga.h and ga.c to your src/ folder and include ga.h in your primary app file. Then set up app messages by calling __app_message_open()__.

You can then track stuff like this:

    trackScreen("screen name");
    trackEvent("category", "action");
    
Keep in mind that you are responsible of handling messages. If you track to many times in a short interval on the watch, the messages could get lost. I recommend implementing some sort of message queue.
