var Analytics = function(analyticsId, appName, appVersion) {
  this.analyticsId = analyticsId;
  this.appName = appName;
  this.appVersion = appVersion;
  this.accountToken = Pebble.getAccountToken();
  
  if (this.accountToken.length > 0) {
    this.analyticsUserId = this.accountToken;
  }
  else {
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
  req.setRequestHeader('Content-length', trackingParams.length);
  req.send(trackingParams);

  console.log('tracking '+ type);
}

Analytics.prototype.trackScreen = function (screenName) {
  this._trackGA('appview', {'cd': screenName});
}

Analytics.prototype.trackEvent = function (category, action) {
  this._trackGA('event', {'ec': category, 'ea': action});
}

// If require() will work some day. This would be nessecary.
module.exports = Analytics;