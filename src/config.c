#include <config.h>

Theme theme;

void load_default_theme() {
  // Override these to change color before compiling
  theme.MainBg = GColorWhite;
  theme.CurrentHourBg = GColorBlack;
  theme.CurrentMinuteFg = GColorWhite;
  theme.OuterRingFg = GColorLightGray;
  theme.OuterRingCardinalFg = GColorDarkGray;
  theme.CurrentMonthOutlineFg = GColorRed;
  theme.CurrentMonthFillBg = GColorChromeYellow; // No Effect as of now
  theme.MiddleRingMarkerFg = GColorBlack;
  theme.MiddleRingFg = GColorDarkGray;
  theme.CurrentDayBg = GColorBlack;
  theme.CurrentDayOutlineFg = GColorDarkGray;
  theme.InnerRingFg = GColorDarkGray;
  theme.CurrentDoWFillBg = GColorBlack;
  theme.LeapYearFg = GColorRed;
  theme.NonLeapYearFg = GColorBlack;
}

