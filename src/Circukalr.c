#include <pebble.h>

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

  for(int i = 0; i < 12; i++) {
    int hour_angle = get_angle_for_hour(i);
    graphics_context_set_text_color(ctx, GColorWhite);

    GPoint pos = gpoint_from_polar(month_hour_ring, GOvalScaleModeFitCircle, DEG_TO_TRIGANGLE(hour_angle));
    graphics_context_set_fill_color(ctx, GColorBlack);
    if (i == s_last_time.month) {
      
      if (s_last_time.h12 == s_last_time.month) {
        graphics_context_set_stroke_color(ctx,GColorFolly);
        graphics_context_set_fill_color(ctx, GColorLightGray);
        graphics_context_set_stroke_width(ctx, 2);
        graphics_draw_circle(ctx, pos, 16);   
      } else {
        graphics_context_set_stroke_color(ctx, GColorFolly);
        graphics_context_set_fill_color(ctx, GColorFolly);
        graphics_context_set_stroke_width(ctx, 2);
        graphics_draw_circle(ctx, pos, 15);
        // graphics_fill_circle(ctx, pos, 14); 
      }

    } else {
      graphics_context_set_stroke_width(ctx, 1);
      if (i % 3 == 0) {
        graphics_context_set_stroke_color(ctx, GColorBlack);
        graphics_context_set_stroke_width(ctx, 1);
      } else {
        graphics_context_set_stroke_color(ctx, GColorLightGray);
      }
      graphics_draw_circle(ctx, pos, 15);
    }
    
    if (i == s_last_time.h12) {
      graphics_context_set_fill_color(ctx, GColorBlack);

      graphics_fill_circle(ctx, pos, 15);
      GRect kte = grect_centered_from_polar(month_hour_ring, GOvalScaleModeFitCircle, DEG_TO_TRIGANGLE(hour_angle), GSize(26,26));
      graphics_draw_text(ctx, m_buffer+((' ' == m_buffer[0])?1:0), fonts_get_system_font(FONT_KEY_LECO_20_BOLD_NUMBERS), kte,
                     GTextOverflowModeWordWrap, GTextAlignmentCenter, s_attributes);
    }
  }
  
  GRect day_ring = grect_inset(bounds, GEdgeInsets(47));
  for(int i = 1; i < 31; i++) {
    int ri;
//     APP_LOG(APP_LOG_LEVEL_DEBUG, "%d", i);

    int day_angle = get_angle_for_day(i);
    graphics_context_set_text_color(ctx, GColorWhite);
    
    if (s_last_time.day == 1){
      ri = 30;
    } else if (s_last_time.day == 31) {
      ri = 31;
    } else {
      ri = s_last_time.day - 1; // debug here
    }
//     APP_LOG(APP_LOG_LEVEL_DEBUG, "%d", ri);

    int v = i + 1;

    GPoint pos = gpoint_from_polar(day_ring, GOvalScaleModeFitCircle, DEG_TO_TRIGANGLE(day_angle));
    if (i == ri) {
      graphics_context_set_fill_color(ctx, GColorBlack);
      graphics_fill_circle(ctx, pos, 3);
      
      if (v % 4 == 0 || i == ri || i == 30){
        graphics_context_set_stroke_width(ctx, 1);
        graphics_context_set_stroke_color(ctx, GColorBlack);
      } else {
        graphics_context_set_stroke_width(ctx, 1);
        graphics_context_set_stroke_color(ctx, GColorLightGray);
      }
      graphics_draw_circle(ctx, pos, 3);
    } else {
      if (v % 4 == 0 || i == ri || i == 30){
        graphics_context_set_stroke_width(ctx, 1);
        graphics_context_set_stroke_color(ctx, GColorBlack);
      } else {
        graphics_context_set_stroke_width(ctx, 1);
        graphics_context_set_stroke_color(ctx, GColorLightGray);
      }
      graphics_draw_circle(ctx, pos, 3);
    }
    
    if (i == 30) {
      GPoint pos_31 = GPoint(pos.x, pos.y + 8);
      if (ri == 31) {
        graphics_context_set_stroke_width(ctx, 1);
        graphics_context_set_stroke_color(ctx, GColorBlack);
        graphics_context_set_fill_color(ctx, GColorBlack);
        graphics_fill_circle(ctx, pos_31, 2);
        graphics_draw_circle(ctx, pos_31, 3);
      } else {
        graphics_context_set_stroke_width(ctx, 1);
        graphics_context_set_stroke_color(ctx, GColorDarkGray);
        graphics_draw_circle(ctx, pos_31, 3);
      }

    }

//     if (i == 30) {
//       GPoint pos_31 = GPoint(pos.x, pos.y + 8);
//       if (s_last_time.day == 31) {
//         graphics_context_set_fill_color(ctx, GColorChromeYellow);
//         graphics_fill_circle(ctx, pos_31, 3);
//       } else {
//         graphics_context_set_fill_color(ctx, GColorBlack);
//         graphics_fill_circle(ctx, pos_31, 3);
//       }
//       graphics_fill_circle(ctx, pos, 3);

//     } else {
//       if (i == 30) {
//         graphics_context_set_fill_color(ctx, GColorChromeYellow);
//       } else {
//         graphics_context_set_fill_color(ctx, GColorBlack);
//       }
//       graphics_fill_circle(ctx, pos, 3);
//     }
  }


  
  GRect week_ring = grect_inset(bounds, GEdgeInsets(69));
  for(int i = 0; i < 8; i++) {
    int dow_angle = get_angle_for_weekday(i);
    GPoint pos = gpoint_from_polar(week_ring, GOvalScaleModeFitCircle, DEG_TO_TRIGANGLE(dow_angle));
    graphics_context_set_stroke_color(ctx, GColorDarkGray);
    graphics_context_set_stroke_width(ctx, 1);
    graphics_draw_circle(ctx, pos, 5);
    
    if (s_last_time.week_day == i) {
      graphics_context_set_fill_color(ctx, GColorBlack);
      graphics_fill_circle(ctx, pos, 5);
    }
  }
  
  GRect pm_box = grect_inset(bounds, GEdgeInsets(76));
  if (is_leap_year(s_last_time.year)) {
        graphics_context_set_text_color(ctx, GColorRed);
  } else {
        graphics_context_set_text_color(ctx, GColorBlack);
  }
  char ap[] = "P";
  if (s_last_time.ap == 1) {
    strcpy(ap, "A");
  } else {
    strcpy(ap, "P");
  }
  graphics_draw_text(ctx, ap, fonts_get_system_font(FONT_KEY_LECO_20_BOLD_NUMBERS), pm_box,
                     GTextOverflowModeWordWrap, GTextAlignmentCenter, s_attributes);

  // Set the fill color
//   graphics_context_set_fill_color(ctx, GColorBlue);
  
//   GPoint start = GPoint(50, 10);
//   GPoint end = GPoint(60, 60);
//   graphics_draw_line(ctx, start, end);
}
static void tick_handler(struct tm *tick_time, TimeUnits changed) {
  
  
	s_last_time.h24 = tick_time->tm_hour;
  if (tick_time->tm_hour > 12) {
    s_last_time.h12 = s_last_time.h24 - 12;
  } else {
    s_last_time.h12 = tick_time->tm_hour;
  }
  
//   s_last_time.minutes = tick_time->tm_min;
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
}


void handle_init(void) {
  my_window = window_create();
  window_set_background_color(my_window, PBL_IF_COLOR_ELSE(GColorWhite, GColorBlack));

  Layer *window_layer = window_get_root_layer(my_window);
  GRect bounds = layer_get_bounds(window_layer);
  draw_layer = layer_create(bounds);
  layer_set_update_proc(draw_layer, canvas_update_proc);
  
  s_attributes = graphics_text_attributes_create();
  
  layer_add_child(window_layer, draw_layer);
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  window_stack_push(my_window, true);
  
}


void handle_deinit(void) {
  window_destroy(my_window);
  layer_destroy(draw_layer);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
