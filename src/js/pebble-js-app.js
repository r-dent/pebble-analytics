/**
 *  Analytics code
 *  Imagine this would be imported from, let´s say, pebble-ga.js.
 */

var Analytics = function(analyticsId, appName, appVersion) {
  this.analyticsId = analyticsId;
  this.appName = appName;
  this.appVersion = appVersion;
  var watch = Pebble.getActiveWatchInfo ? Pebble.getActiveWatchInfo() : null;
  var userAgent = 'Mozilla/5.0';
  if (watch) {
      var fw = watch.firmware;
      var fw_ver = fw.major + '.' + fw.minor + '.' + fw.patch + (fw.suffix ? '-' + fw.suffix : '');
      userAgent += ' (Pebble; ' + watch.platform + ' ' + fw_ver + '; ' + watch.model + '; ' + watch.language + ') PebbleKitJS/' + fw_ver;
  } else {
      userAgent += ' (Pebble; unknown) PebbleKitJS/unknown';
  }
  console.log("User-Agent", userAgent);
  this.userAgent = userAgent;
  var accountToken = Pebble.getAccountToken();
  if (accountToken.length > 0) {
    this.analyticsUserId = accountToken;
  } else {
    this.analyticsUserId = window.localStorage.getItem('analyticsUserId');
    if (this.analyticsUserId == undefined) {
      this.analyticsUserId = Math.round(Math.random() * 1000000000);
      window.localStorage.setItem('analyticsUserId', this.analyticsUserId);
    }
  }
}

Analytics.prototype._trackGA = function(type, params) {
  var req = new XMLHttpRequest();
  var url = 'http://www.google-analytics.com/collect';
  var trackingParams = 'v=1'
  + '&tid=' + this.analyticsId
  + '&cid=' + this.analyticsUserId
  + '&t=' + type
  + '&an=' + this.appName
  + '&av=' + this.appVersion;

  for (parameterKey in params) {
    if (params.hasOwnProperty(parameterKey)) {
      trackingParams += '&'+ parameterKey +'='+ params[parameterKey];
    }
  }

  req.open('POST', url, true);
  req.setRequestHeader('User-Agent', this.userAgent);
  req.setRequestHeader('Content-Length', trackingParams.length);
  req.send(trackingParams);
  console.log('tracking', type, JSON.stringify(params));
}

Analytics.prototype.trackScreen = function (screenName) {
  this._trackGA('appview', {'cd': screenName});
}

Analytics.prototype.trackEvent = function (category, action) {
  this._trackGA('event', {'ec': category, 'ea': action});
}

/**
 *  End of Analytics code
 *  Imagine this were imported from, let´s say, pebble-ga.js.
 */

// If require() will work some day. This would be the way.
// require('pebble-ga');

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
