#include <pebble.h>
#include <config.h>


Window *my_window;
Layer *draw_layer;
typedef struct {
  int hours;
  int h12;
  int h24;
  int month;
  int minutes;
  int week_day;
  int day;
  int year;
  int ap;
} Time;
static Time s_last_time;
static GTextAttributes *s_attributes;
char m_buffer[] = "59";
char ap_buffer[] = "AM";

// Our Settings
Theme theme;
Settings settings;



static int32_t get_angle_for_hour(int hour) {
  // Progress through 12 hours, out of 360 degrees
  return (hour * 360) / 12;
}

static int32_t get_angle_for_day(int day) {
  // Progress through 30 days, out of 360 degrees
  return (day * 360) / 30;
}

static int32_t get_angle_for_weekday(int day) {
  // Progress through 30 days, out of 360 degrees
  return (day * 360) / 7;
}

static bool is_leap_year(int year) {
  if ( year%400 == 0)
    return true;
  else if ( year%100 == 0)
    return 0;
  else if ( year%4 == 0 )
    return true;
  else
    return 0;
 
  return 0;
}

static void canvas_update_proc(Layer *layer, GContext *ctx) {
    // Set the line color
  GRect bounds = layer_get_bounds(layer);
  
  GRect month_hour_ring = grect_inset(bounds, GEdgeInsets(22));

  graphics_context_set_stroke_color(ctx, GColorRed);

  for(int i = 0; i < 13; i++) {
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "%d", i);
//     APP_LOG(APP_LOG_LEVEL_DEBUG, "month is %d", s_last_time.month);
//

    int hour_angle = get_angle_for_hour(i);
    graphics_context_set_text_color(ctx, theme.CurrentMinuteFg);

    GPoint pos = gpoint_from_polar(month_hour_ring, GOvalScaleModeFitCircle, DEG_TO_TRIGANGLE(hour_angle));

    if (persist_read_int(SUNRISE_KEY)) {
      int vv;
      int vap;
      time_t sunrise_t =   persist_read_int(SUNRISE_KEY);
      struct tm *sunrise_time = localtime(&sunrise_t);
      if (sunrise_time->tm_hour > 12) {
        vv = sunrise_time->tm_hour - 12;
      } else {
        vv = sunrise_time->tm_hour;
      }
      strftime(ap_buffer, sizeof(ap_buffer), "%p", sunrise_time);
      if (! strcmp(ap_buffer, "AM")) {
        vap = 1;
      } else {
        vap = 2;
      }
      if (vv == i && vap == s_last_time.ap && 
          i != s_last_time.month && vv != s_last_time.h12) {
        // Just outline if nothing conflicts
        graphics_context_set_fill_color(ctx, theme.SunriseFillBg);
        graphics_fill_circle(ctx, pos, 16);   
      } else if (vv == i && vap == s_last_time.ap 
          && i == s_last_time.month && s_last_time.h12 != vv) {
        // sunrise is on the same marker as the month but not hour
        // Just draw
        graphics_context_set_stroke_width(ctx, 2);
        graphics_context_set_stroke_color(ctx, theme.SunriseOutlineFg);
        graphics_draw_circle(ctx, pos, 16);   
      } else if (vv == i && vap == s_last_time.ap 
          && i == s_last_time.month && s_last_time.h12 != vv) {
        // same month, and hour, just do nothing
      }
    }

    if (persist_read_int(SUNSET_KEY)) {
      int vv;
      int vap;
      time_t sunset_t =   persist_read_int(SUNSET_KEY);
      struct tm *sunset_time = localtime(&sunset_t);
      if (sunset_time->tm_hour > 12) {
        vv = sunset_time->tm_hour - 12;
      } else {
        vv = sunset_time->tm_hour;
      }
      strftime(ap_buffer, sizeof(ap_buffer), "%p", sunset_time);
      if (! strcmp(ap_buffer, "AM")) {
        vap = 1;
      } else {
        vap = 2;
      }
      if (vv == i && vap == s_last_time.ap && 
          i != s_last_time.month && vv != s_last_time.h12) {
        // Fill it if theres no conflict
        graphics_context_set_fill_color(ctx, theme.SunsetFillBg);
        graphics_fill_circle(ctx, pos, 14); 
      } else if (vv == i && vap == s_last_time.ap 
          && i == s_last_time.month && s_last_time.h12 != vv) {
        // sunrise is on the same marker as the month but not hour
        // Just draw
        graphics_context_set_stroke_color(ctx, theme.SunsetOutlineFg);
        graphics_context_set_stroke_width(ctx, 2);
        graphics_draw_circle(ctx, pos, 16);   
      } else if (vv == i && vap == s_last_time.ap 
          && i == s_last_time.month && s_last_time.h12 != vv) {
        // same month, and hour, just do nothing
      }
    }
    if (i == s_last_time.month + 1) {
      
      if (s_last_time.h12 == s_last_time.month + 1) {
        // Case where time and month are the same
        graphics_context_set_stroke_color(ctx, theme.CurrentMonthOutlineFg);
        graphics_context_set_stroke_width(ctx, 2);
        graphics_draw_circle(ctx, pos, 16);   
      } else {
        graphics_context_set_stroke_color(ctx, theme.CurrentMonthOutlineFg);
        graphics_context_set_fill_color(ctx, theme.CurrentMonthFillBg);
        graphics_context_set_stroke_width(ctx, 2);
        graphics_draw_circle(ctx, pos, 15);
        graphics_fill_circle(ctx, pos, 14); 
      }

    } else {
      graphics_context_set_stroke_width(ctx, 1);
      if (i % 3 == 0) {
        graphics_context_set_stroke_color(ctx, theme.OuterRingCardinalFg);
        graphics_context_set_stroke_width(ctx, 1);
      } else {
        graphics_context_set_stroke_color(ctx, theme.OuterRingFg);
      }
      graphics_draw_circle(ctx, pos, 15);
    }
    
    if (i == s_last_time.h12) {
      graphics_context_set_fill_color(ctx, theme.CurrentHourBg);
      graphics_fill_circle(ctx, pos, 15);
      GRect kte = grect_centered_from_polar(month_hour_ring, GOvalScaleModeFitCircle, DEG_TO_TRIGANGLE(hour_angle), GSize(26,26));
      graphics_draw_text(ctx, m_buffer+((' ' == m_buffer[0])?1:0), fonts_get_system_font(FONT_KEY_LECO_20_BOLD_NUMBERS), kte,
                     GTextOverflowModeWordWrap, GTextAlignmentCenter, s_attributes);
    }
  }
  
  GRect day_ring = grect_inset(bounds, GEdgeInsets(47));
  for(int i = 1; i < 31; i++) {

    int day_angle = get_angle_for_day(i);
    
//     if (s_last_time.day == 1){
//       ri = 30;
//     } else if (s_last_time.day == 31) {
//       ri = 31;
//     } else {
//       ri = s_last_time.day - 1; // debug here
//     }
//     APP_LOG(APP_LOG_LEVEL_DEBUG, "%d", ri);

//     int v = i + 1;

    GPoint pos = gpoint_from_polar(day_ring, GOvalScaleModeFitCircle, DEG_TO_TRIGANGLE(day_angle));
    if (i == s_last_time.day) {
      graphics_context_set_fill_color(ctx, theme.CurrentDayBg);
      graphics_fill_circle(ctx, pos, 3);
      
      if (i % 5 == 0){
        graphics_context_set_stroke_width(ctx, 1);
        graphics_context_set_stroke_color(ctx, theme.MiddleRingMarkerFg);
      } else {
        graphics_context_set_stroke_width(ctx, 1);
        graphics_context_set_stroke_color(ctx, theme.MiddleRingFg);
      }
      graphics_draw_circle(ctx, pos, 3);
    } else {
      if (i % 5 == 0){
        graphics_context_set_stroke_width(ctx, 1);
        graphics_context_set_stroke_color(ctx, theme.MiddleRingMarkerFg);
      } else {
        graphics_context_set_stroke_width(ctx, 1);
        graphics_context_set_stroke_color(ctx, theme.MiddleRingFg);
      }
      graphics_draw_circle(ctx, pos, 3);
    }
    
    if (i == 30) {
      GPoint pos_31 = GPoint(pos.x, pos.y + 8);
      if (s_last_time.day == 31) {
        graphics_context_set_stroke_width(ctx, 1);
        graphics_context_set_stroke_color(ctx, theme.CurrentDayOutlineFg);
        graphics_context_set_fill_color(ctx, theme.CurrentDayBg);
        graphics_fill_circle(ctx, pos_31, 2);
        graphics_draw_circle(ctx, pos_31, 3);
      } else {
        graphics_context_set_stroke_width(ctx, 1);
        graphics_context_set_stroke_color(ctx, theme.MiddleRingFg);
        graphics_draw_circle(ctx, pos_31, 3);
      }

    }
  }


  
  GRect week_ring = grect_inset(bounds, GEdgeInsets(69));
  for(int i = 0; i < 7; i++) {
    int dow_angle = get_angle_for_weekday(i);
    GPoint pos = gpoint_from_polar(week_ring, GOvalScaleModeFitCircle, DEG_TO_TRIGANGLE(dow_angle));
    graphics_context_set_stroke_color(ctx, theme.InnerRingFg);
    graphics_context_set_stroke_width(ctx, 1);
    graphics_draw_circle(ctx, pos, 5);
    
    if (s_last_time.week_day == i) {
      graphics_context_set_fill_color(ctx, theme.CurrentDoWFillBg);
      graphics_fill_circle(ctx, pos, 5);
    }
  }
  
  GRect pm_box = grect_inset(bounds, GEdgeInsets(76));
  if (is_leap_year(s_last_time.year)) {
        graphics_context_set_text_color(ctx, theme.LeapYearFg);
  } else {
        graphics_context_set_text_color(ctx, theme.NonLeapYearFg);
  }
  char ap[] = "P";
  if (s_last_time.ap == 1) {
    strcpy(ap, "A");
  } else {
    strcpy(ap, "P");
  }
  graphics_draw_text(ctx, ap, fonts_get_system_font(FONT_KEY_LECO_20_BOLD_NUMBERS), pm_box,
                     GTextOverflowModeWordWrap, GTextAlignmentCenter, s_attributes);
}
static void tick_handler(struct tm *tick_time, TimeUnits changed) {

  
  s_last_time.h24 = tick_time->tm_hour;
    if (tick_time->tm_hour > 12) {
    s_last_time.h12 = s_last_time.h24 - 12;
  } else {
    s_last_time.h12 = tick_time->tm_hour;
  }
  strftime(m_buffer, sizeof(m_buffer), "%M", tick_time);
  s_last_time.month = tick_time->tm_mon;
  s_last_time.week_day = tick_time->tm_wday;
  s_last_time.day = tick_time->tm_mday;
  s_last_time.year = tick_time->tm_year;
  strftime(ap_buffer, sizeof(ap_buffer), "%p", tick_time);
  if (! strcmp(ap_buffer, "AM")) {
      s_last_time.ap = 1;
  } else {
    s_last_time.ap = 2;
  }

  layer_mark_dirty(draw_layer);
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Set: %d", (int)settings.SUNRISE);
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Per: %d", (int)persist_read_int(SUNRISE_KEY));
}

void save_config() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings)); 
  layer_mark_dirty(draw_layer);
}

void inbox_recieved(DictionaryIterator *iter, void *context) {
  Tuple *sunrise = dict_find(iter, MESSAGE_KEY_SUNRISE);
  if (sunrise) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Sunrise Recieved: %d", (int)sunrise->value->int32);
    settings.SUNRISE = sunrise->value->int32;
    persist_write_int(SUNRISE_KEY, (int)settings.SUNRISE);
  }

  Tuple *sunset = dict_find(iter, MESSAGE_KEY_SUNSET);
  if (sunset) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Sunset Recieved: %d", (int)sunset->value->int32);
    settings.SUNSET = sunset->value->int32;
    persist_write_int(SUNSET_KEY, (int)settings.SUNSET);
  }
  save_config();
}


void handle_init(void) {
  app_message_register_inbox_received(inbox_recieved);
  app_message_open(128, 128);
  //light_enable(true);
  my_window = window_create();
  load_default_theme();
  window_set_background_color(my_window, theme.MainBg);

  Layer *window_layer = window_get_root_layer(my_window);
  GRect bounds = layer_get_bounds(window_layer);
  draw_layer = layer_create(bounds);
  layer_set_update_proc(draw_layer, canvas_update_proc);
  
  s_attributes = graphics_text_attributes_create();
  
  layer_add_child(window_layer, draw_layer);
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  window_stack_push(my_window, true);

  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Set: %d", (int)settings.SUNRISE);
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Per: %d", (int)persist_read_int(SUNRISE_KEY));
  
}


void handle_deinit(void) {
  app_message_deregister_callbacks(); 
  window_destroy(my_window);
  layer_destroy(draw_layer);
  graphics_text_attributes_destroy(s_attributes);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
