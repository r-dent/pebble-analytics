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

module.exports = Analytics;
