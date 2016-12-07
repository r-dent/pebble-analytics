
var Analytics = require('./pebble-ga');
var ga = new Analytics('UA-1234567-0', 'myApp', '1.0.0');

var messageType = {
  "track_screen" : 0,
  "track_event" : 1
}

Pebble.addEventListener('ready', function(e) {
  console.log('JavaScript app ready and running!');

  ga.trackEvent('lifecycle', 'start App');
});

Pebble.addEventListener('appmessage', function(e) {
  if (e.payload['message_type'] !== undefined) {
    switch(e.payload['message_type']) {

      case messageType['track_screen']:
        ga.trackScreen(e.payload['track_screen_name']);
        break;

      case messageType['track_event']:
        ga.trackEvent(e.payload['track_event_category'], e.payload['track_event_action']);
        break;

    }
  } 
});
