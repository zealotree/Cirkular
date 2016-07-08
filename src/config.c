#include <pebble.h>
#include <config.h>

Theme theme;

void load_default_theme() {
  // Override these to change color before compiling
  theme.MainBg = GColorBlack;
  theme.CurrentHourBg = GColorWhite;
  theme.CurrentMinuteFg = GColorBlack;
  theme.OuterRingFg = GColorBlack;
  theme.OuterRingCardinalFg = GColorWhite;
  theme.CurrentMonthOutlineFg = GColorRed;
  theme.CurrentMonthFillBg = GColorChromeYellow;
  theme.MiddleRingMarkerFg = GColorMelon;
  theme.MiddleRingFg = GColorDarkGray;
  theme.CurrentDayBg = GColorChromeYellow;
  theme.CurrentDayOutlineFg = GColorChromeYellow;
  theme.InnerRingFg = GColorDarkGray;
  theme.CurrentDoWFillBg = GColorChromeYellow ;
  theme.LeapYearFg = GColorMelon ;
  theme.NonLeapYearFg = GColorWhite ;
}

Settings settings;
void load_default_settings() {
  settings.ENABLE_SUN = true; // Gets current location
}



