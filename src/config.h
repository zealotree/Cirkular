#include <pebble.h>
// Define your defaults here
typedef struct {
  GColor MainBg;
  GColor OuterRingFg;
  GColor CurrentHourOutlineFg;
  GColor OuterRingBg;
  GColor OuterRingCardinalFg;
  GColor OuterRingCardinalBg;
  GColor CurrentMonthOutlineFg;
  GColor CurrentMonthFillBg;
  GColor CurrentHourBg;
  GColor OuterRingEmptyBg;
  GColor CurrentMinuteFg;
  GColor MiddleRingFg;
  GColor MiddleRingMarkerFg;
  GColor MiddleRingEmptyBg;
  GColor CurrentDayBg;
  GColor CurrentDayOutlineFg;
  GColor InnerRingFg;
  GColor CurrentDoWFillBg;
  GColor LeapYearFg;
  GColor NonLeapYearFg;
}  __attribute__((__packed__)) Theme;

void load_default_theme();
