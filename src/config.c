#include <config.h>

Theme theme;

void load_default_theme() {
  // Override these to change color before compiling
  theme.MainBg = GColorWhite;
  theme.CurrentHourBg = GColorBlack;
  theme.CurrentHourOutlineFg = GColorBlack;
  theme.CurrentMinuteFg = GColorWhite;
  theme.OuterRingFg = GColorDarkGray;
  theme.OuterRingEmptyBg = GColorClear;
  theme.OuterRingCardinalFg = GColorBlack;
  theme.CurrentMonthOutlineFg = GColorRed;
  theme.CurrentMonthFillBg = GColorChromeYellow; // No Effect as of now
  theme.MiddleRingMarkerFg = GColorBlack;
  theme.MiddleRingEmptyBg = GColorClear;
  theme.MiddleRingFg = GColorDarkGray;
  theme.CurrentDayBg = GColorBlack;
  theme.CurrentDayOutlineFg = GColorBlack;
  theme.InnerRingFg = GColorBlack;
  theme.CurrentDoWFillBg = GColorBlack;
  theme.LeapYearFg = GColorRed;
  theme.NonLeapYearFg = GColorBlack;
}

