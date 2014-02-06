/**
 *  Analytics code
 */

var googleAnalyticsId = 'UA-1234567-4';
var analyticsUserId = Math.round(Math.random() * 10000000);
var analyticsAppName = 'myApp';
var version = '0.1';

var messageType = {
  "track_screen" : 0,
  "track_event" : 1
}

function _trackGA (type, params) {
  var req = new XMLHttpRequest();
  var url = 'http://www.google-analytics.com/collect';
  var trackingParams = 'v=1'
  + '&tid=' + googleAnalyticsId
  + '&cid=' + analyticsUserId
  + '&t=' + type
  + '&an=' + analyticsAppName
  + '&av=' + version;

  for (parameterKey in params) {
    if (params.hasOwnProperty(parameterKey)) {
      trackingParams += '&'+ parameterKey +'='+ params[parameterKey];
    }
  }

  req.open('POST', url, true);
  req.setRequestHeader('Content-length', trackingParams.length);
  req.send(trackingParams);
}

function trackScreen (screenName) {
  _trackGA('appview', {'cd': screenName});
}

function trackEvent (category, action) {
  _trackGA('event', {'ec': category, 'ea': action});
}

Pebble.addEventListener('ready', function(e) {
  console.log('JavaScript app ready and running!');
  console.log("Pebble Account Token: " + Pebble.getAccountToken());

  //Pebble.showSimpleNotificationOnPebble('Ein tiitel!', 'I think you are "'+ Pebble.getAccountToken() +'"');
  trackEvent('lifecycle', 'start App');
});

Pebble.addEventListener('appmessage', function(e) {
  if (e.payload['message_type'] !== undefined) {
    switch(e.payload['message_type']) {

      case messageType['track_screen']:
        trackScreen(e.payload['track_screen_name']);
        break;

      case messageType['track_event']:
        trackEvent(e.payload['track_event_category'], e.payload['track_event_action']);
        break;

    }
  } 
});