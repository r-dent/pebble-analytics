pebble-analytics
================

Google Analytics implementation for Pebble smartwatch.

## Usage

###On the JS side

1. Copy contents of pebble-ga.js to the beginning of pebble-js-app.js
2. Initialize an analytics object like this

    var ga = new Analytics('UA-1234567-0', 'myApp', '1.0.0');

3. Track events or screen with like this.

    ga.trackScreen('screen name');
    ga.trackEvent('category', 'action');

###On the C side

1. Copy ga.h and ga.c to your src/ folder.
2. Include ga.h in your app.c
3. Set up app messages by calling app_message_open()
4. Track content like this

    trackScreen("screen name");
    trackEvent("category", "action");