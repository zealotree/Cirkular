#include <pebble.h>
// Define your defaults here

#define SETTINGS_KEY 1
#define SUNRISE_KEY 2
#define SUNSET_KEY 3
#define SEND 900

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

typedef struct {
  bool ENABLE_SUN;
  int SUNRISE;
  int SUNSET;
}  __attribute__((__packed__)) Settings;

void load_default_theme();
void load_default_settings();
void save_config();
void inbox_recieved();
