#include <pebble.h>
#include <config.h>

Theme theme;
void load_default_theme() {
  // Override these to change color before compiling
  theme.MainBg = GColorBlack;
  theme.OuterRingEmptyBg = GColorClear;
  theme.MiddleRingEmptyBg = GColorClear;
  theme.CurrentHourBg = GColorWhite;
  theme.CurrentMinuteFg = GColorBlack;
  theme.OuterRingFg = GColorDarkGray; // Hour Rings
  theme.OuterRingCardinalFg = GColorWhite; // Cardinal Hour Rings
  theme.CurrentMonthOutlineFg = GColorRed;
  theme.CurrentMonthFillBg = GColorChromeYellow;
  theme.MiddleRingMarkerFg = GColorMelon;
  theme.MiddleRingFg = GColorDarkGray;
  theme.CurrentDayBg = GColorChromeYellow;
  theme.CurrentDayOutlineFg = GColorChromeYellow;
  theme.InnerRingFg = GColorDarkGray;
  theme.CurrentDoWFillBg = GColorChromeYellow ;
  theme.LeapYearFg = GColorRed;
  theme.NonLeapYearFg = GColorWhite ;
  theme.SunriseFillBg = GColorIcterine;
  theme.SunriseOutlineFg = GColorBlue;
  theme.SunsetFillBg = GColorSunsetOrange;
  theme.SunsetOutlineFg = GColorIcterine;
}

Settings settings;
void load_default_settings() {
}



