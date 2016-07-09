var SunCalc = require('./suncalc');

var lat;
var lon;


function setPos(pos) {
  console.log(typeof pos.coords.latitdue);
  console.log("Success -- setting coords")
  var times = SunCalc.getTimes(new Date(), pos.coords.latitude, pos.coords.longitude);
  localStorage.setItem('lastLAT', pos.coords.longitude);
  localStorage.setItem('lastLON', pos.coords.latitude);
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
    if (dict["MESSAGE_KEY_SEND"] || dict["SEND"]) {
      navigator.geolocation.getCurrentPosition(
        setPos,
        noSetPos,
        {timeout: 6000, maximumAge: 60000, enableHighAccuracy: true}
      );
    }
});

