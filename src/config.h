#include <pebble.h>
// Define your defaults here
typedef struct {
  GColor MainBg;
  GColor OuterRingFg;
  GColor OuterRingBg;
  GColor OuterRingCardinalFg;
  GColor OuterRingCardinalBg;
  GColor CurrentMonthOutlineFg;
  GColor CurrentMonthFillBg;
  GColor CurrentHourBg;
  GColor CurrentMinuteFg;
  GColor MiddleRingFg;
  GColor MiddleRingMarkerFg;
  GColor CurrentDayBg;
  GColor CurrentDayOutlineFg;
  GColor InnerRingFg;
  GColor CurrentDoWFillBg;
  GColor LeapYearFg;
  GColor NonLeapYearFg;
}  __attribute__((__packed__)) Theme;

void load_default_theme();
