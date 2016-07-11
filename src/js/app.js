var Clay = require('pebble-clay');
var clayConfig = require('./config');
var clay = new Clay(clayConfig);  

var SunCalc = require('./suncalc');

function setPos(pos) {
  console.log("Success -- setting coords")
  var times = SunCalc.getTimes(new Date(), pos.coords.latitude, pos.coords.longitude);
  var sunrise = parseInt(times.sunrise.getTime()/1000);
  var sunset = parseInt(times.sunset.getTime()/1000);
  Pebble.sendAppMessage({'SUNRISE' : sunrise});
  Pebble.sendAppMessage({'SUNSET' : sunset});
}

function noSetPos(pos) {
  console.log("Failed getting location. Location must be enabled.")
}

Pebble.addEventListener('ready', function() {

  // Update s_js_ready on watch
  Pebble.sendAppMessage({'AppKeyJSReady': 1});
  console.log("Ready received!");

});


Pebble.addEventListener('appmessage', function(e) {
    console.log("AppMessage received!");
    var dict = e.payload;
    console.log(JSON.stringify(dict));
    if (dict["SEND"] && dict["USE_GPS"] == 1) {
      navigator.geolocation.getCurrentPosition(
        setPos,
        noSetPos,
        {timeout: 6000, maximumAge: 60000, enableHighAccuracy: true}
      );
    }
    if (dict["SEND"]) {
      if (dict["LAT"] || dict["LON"]) {
        var lat = parseFloat(dict["LAT"]);
        var lon = parseFloat(dict["LON"]);
        var times = SunCalc.getTimes(new Date(), lat, lon);
        var sunrise = parseInt(times.sunrise.getTime()/1000);
        var sunset = parseInt(times.sunset.getTime()/1000);
        console.log("Sending Fixed Location");
        Pebble.sendAppMessage({'SUNRISE' : sunrise});
        Pebble.sendAppMessage({'SUNSET' : sunset});
      }
    }
});

