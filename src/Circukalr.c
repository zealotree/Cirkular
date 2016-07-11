#include <pebble.h>
#include <config.h>


Window *my_window;
Layer *draw_layer;
Layer *sun_layer;
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
bool s_js_ready = false;

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

void request_data() {
  if (! s_js_ready) {
    return ;
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Requesting Data");
  DictionaryIterator *out_iter;
  AppMessageResult result = app_message_outbox_begin(&out_iter);
  if(result == APP_MSG_OK) {
    int value = 1;
    dict_write_int(out_iter, MESSAGE_KEY_SEND, &value, sizeof(int), true);
    if (persist_exists(USE_GPS_KEY) && ! persist_read_bool(USE_GPS_KEY)) {
      if (persist_exists(LAT_KEY) && persist_exists(LON_KEY)) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Sending Fixed Location");
        static char temp_buffer[80];
        static char temp_buffer2[80];

        persist_read_string(LAT_KEY, temp_buffer, sizeof(temp_buffer));
        dict_write_cstring(out_iter, MESSAGE_KEY_LAT, temp_buffer);  

        persist_read_string(LON_KEY, temp_buffer2, sizeof(temp_buffer2));
        dict_write_cstring(out_iter, MESSAGE_KEY_LON, temp_buffer2);  
      }
    } else if (persist_exists(USE_GPS_KEY) && persist_read_bool(USE_GPS_KEY)) {
      dict_write_int(out_iter, MESSAGE_KEY_USE_GPS, &value, sizeof(int), true);
    }
    result = app_message_outbox_send();
    if(result != APP_MSG_OK) {
      APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the outbox: %d", (int)result);
    } else if ((result == APP_MSG_OK)){
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Data succesfully requested");
    }
  }
}
static void up_sun(Layer *layer, GContext *ctx) {

  GRect bounds = layer_get_bounds(layer);
  GRect month_hour_ring = grect_inset(bounds, GEdgeInsets(22));
  for(int i = 0; i < 13; i++) {
    int hour_angle = get_angle_for_hour(i);
    GPoint pos = gpoint_from_polar(month_hour_ring, GOvalScaleModeFitCircle, DEG_TO_TRIGANGLE(hour_angle));
    if (persist_read_int(SUNRISE_KEY)) {
      int h12;
      int temp_ap;
      time_t sunrise_t = persist_read_int(SUNRISE_KEY);
      struct tm *sunrise_time = localtime(&sunrise_t);

      if (sunrise_time->tm_hour > 12) {
        h12 = sunrise_time->tm_hour - 12;
        } else {
        h12 = sunrise_time->tm_hour;
      }

      strftime(ap_buffer, sizeof(ap_buffer), "%p", sunrise_time);
      if (! strcmp(ap_buffer, "AM")) {
        temp_ap = 1;
      } else {
        temp_ap = 2;
      }

      if (i == h12 && temp_ap == s_last_time.ap) {
        graphics_context_set_fill_color(ctx, theme.SunriseFillBg);
        graphics_fill_circle(ctx, pos, 15); 
      }
    }

    if (persist_read_int(SUNSET_KEY)) {
      int h12;
      int temp_ap;
      time_t sunset_t = persist_read_int(SUNSET_KEY);
      struct tm *sunset_time = localtime(&sunset_t);

      if (sunset_time->tm_hour > 12) {
        h12 = sunset_time->tm_hour - 12;
        } else {
        h12 = sunset_time->tm_hour;
      }

      strftime(ap_buffer, sizeof(ap_buffer), "%p", sunset_time);
      if (! strcmp(ap_buffer, "AM")) {
        temp_ap = 1;
      } else {
        temp_ap = 2;
      }

      if (i == h12 && temp_ap == s_last_time.ap) { // Only show either Sunrise or Sunset
        graphics_context_set_fill_color(ctx, theme.SunsetFillBg);
        graphics_fill_circle(ctx, pos, 15); 
      }
    }
  }
}


static void canvas_update_proc(Layer *layer, GContext *ctx) {
    // Set the line color
  GRect bounds = layer_get_bounds(layer);
  
  GRect month_hour_ring = grect_inset(bounds, GEdgeInsets(22));

  graphics_context_set_stroke_color(ctx, GColorRed);

  for(int i = 0; i < 13; i++) {
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "%d", i);
  int hour_angle = get_angle_for_hour(i);

  GPoint pos = gpoint_from_polar(month_hour_ring, GOvalScaleModeFitCircle, DEG_TO_TRIGANGLE(hour_angle));



	if (i == s_last_time.h12) { // Mark the current hour
      graphics_context_set_fill_color(ctx, theme.CurrentHourBg);
      graphics_context_set_stroke_color(ctx, theme.CurrentHourOutlineFg); // Cardinals canot be overriden
      graphics_context_set_stroke_width(ctx, 1);
      graphics_fill_circle(ctx, pos, 14);
      graphics_draw_circle(ctx, pos, 15);
      graphics_context_set_text_color(ctx, theme.CurrentMinuteFg);
      GRect kte = grect_centered_from_polar(month_hour_ring, GOvalScaleModeFitCircle, DEG_TO_TRIGANGLE(hour_angle), GSize(26,26));
      graphics_draw_text(ctx, m_buffer+((' ' == m_buffer[0])?1:0), fonts_get_system_font(FONT_KEY_LECO_20_BOLD_NUMBERS), kte,
                     GTextOverflowModeWordWrap, GTextAlignmentCenter, s_attributes);
    } else if (i % 3 == 0) { // Mark the Cardinal
      graphics_context_set_stroke_color(ctx, theme.OuterRingCardinalFg);
      graphics_context_set_fill_color(ctx, theme.OuterRingEmptyBg);
      graphics_context_set_stroke_width(ctx, 1);
      graphics_draw_circle(ctx, pos, 15);  
      graphics_fill_circle(ctx, pos, 14);
    } else {
      graphics_context_set_stroke_color(ctx, theme.OuterRingFg);
      graphics_context_set_fill_color(ctx, theme.OuterRingEmptyBg);
      graphics_context_set_stroke_width(ctx, 1);
      graphics_draw_circle(ctx, pos, 15);  
      graphics_fill_circle(ctx, pos, 14);
    }

    if (i == s_last_time.month + 1) { // The Current month
      graphics_context_set_stroke_color(ctx, theme.CurrentMonthOutlineFg);
      //graphics_context_set_fill_color(ctx, theme.CurrentMonthFillBg);
      graphics_context_set_stroke_width(ctx, 2);
      graphics_draw_circle(ctx, pos, 15);
      //graphics_fill_circle(ctx, pos, 14);
    }	
   
  }
  
  GRect day_ring = grect_inset(bounds, GEdgeInsets(47));
  for(int i = 1; i < 31; i++) {

    int day_angle = get_angle_for_day(i);
   

    GPoint pos = gpoint_from_polar(day_ring, GOvalScaleModeFitCircle, DEG_TO_TRIGANGLE(day_angle));
    if (i == s_last_time.day) { // Mark the day
      graphics_context_set_fill_color(ctx, theme.CurrentDayBg);
      graphics_context_set_stroke_color(ctx, theme.CurrentDayOutlineFg);
      graphics_context_set_stroke_width(ctx, 1);
      graphics_fill_circle(ctx, pos, 3);
      graphics_draw_circle(ctx, pos, 3);
    } else if (i % 5 == 0) { // Mark the fives
      graphics_context_set_stroke_color(ctx, theme.MiddleRingMarkerFg);
      graphics_context_set_fill_color(ctx, theme.MiddleRingEmptyBg);
      graphics_context_set_stroke_width(ctx, 1);
      graphics_fill_circle(ctx, pos, 3);
      graphics_draw_circle(ctx, pos, 3);  
    } else { // Just fill
      graphics_context_set_stroke_color(ctx, theme.MiddleRingFg);
      graphics_context_set_fill_color(ctx, theme.MiddleRingEmptyBg);
      graphics_context_set_stroke_width(ctx, 1);
      graphics_fill_circle(ctx, pos, 3);
      graphics_draw_circle(ctx, pos, 3);  
    }
    if (i == 30) { // Get the 30th pos
      GPoint pos_31 = GPoint(pos.x, pos.y + 8); // Get 31st post
      if (s_last_time.day == 31) { // Mark it day is 31
        graphics_context_set_stroke_width(ctx, 1);
        graphics_context_set_stroke_color(ctx, theme.CurrentDayBg);
        graphics_context_set_fill_color(ctx, theme.CurrentDayOutlineFg);
        graphics_fill_circle(ctx, pos_31, 3);
        graphics_draw_circle(ctx, pos_31, 3);
      } else {
        graphics_context_set_stroke_width(ctx, 1);
        graphics_context_set_stroke_color(ctx, theme.MiddleRingFg);
        graphics_context_set_fill_color(ctx, theme.MiddleRingEmptyBg);
        graphics_fill_circle(ctx, pos_31, 3);
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

  layer_mark_dirty(draw_layer);
  layer_mark_dirty(sun_layer);
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

  if (DAY_UNIT & changed) {
    request_data();
  }

  if (! persist_exists(SUNRISE_KEY) && ! persist_exists(SUNSET_KEY)) {
    request_data();
  }

}

void save_config() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings)); 
  layer_mark_dirty(draw_layer);
  layer_mark_dirty(sun_layer);
}

void inbox_recieved(DictionaryIterator *iter, void *context) {
  Tuple *ready_tuple = dict_find(iter, MESSAGE_KEY_AppKeyJSReady);

  if(ready_tuple) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "PebbleKit JS is ready! Safe to send messages");
    s_js_ready = true;
    request_data();
  }

  Tuple *ugps = dict_find(iter, MESSAGE_KEY_USE_GPS);
  if (ugps) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "GPS Pref Recieved: %d", (int)ugps->value->int8);
    persist_write_int(USE_GPS_KEY, (int)ugps->value->int8);
  }

  Tuple *lat_tuple = dict_find(iter, MESSAGE_KEY_LAT);
  Tuple *lon_tuple = dict_find(iter, MESSAGE_KEY_LON);
  if (lat_tuple && lon_tuple) {
  	if (strcmp(lat_tuple->value->cstring, "") && strcmp(lon_tuple->value->cstring, "")) {
	    persist_write_string(LAT_KEY, lat_tuple->value->cstring);
	    persist_write_string(LON_KEY, lon_tuple->value->cstring);
    }
  }

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

static void main_window_load(Window *window) {
  load_default_theme();
  window_set_background_color(my_window, theme.MainBg);
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  draw_layer = layer_create(bounds);
  sun_layer = layer_create(bounds);
  layer_set_update_proc(draw_layer, canvas_update_proc);
  layer_set_update_proc(sun_layer, up_sun);
  layer_add_child(window_layer, draw_layer);
  layer_add_child(window_layer, sun_layer);

  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

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
  layer_mark_dirty(sun_layer);

  if (! persist_exists(SUNRISE_KEY) && ! persist_exists(SUNSET_KEY)) {
    request_data();
  } else {
    layer_mark_dirty(sun_layer);
  }
}

static void main_window_unload(Window *window) {
  window_destroy(my_window);
  graphics_text_attributes_destroy(s_attributes);
}


void handle_init(void) {
  app_message_register_inbox_received(inbox_recieved);
  app_message_open(128, 128);
  //light_enable(true);
  my_window = window_create();

  window_set_window_handlers(my_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  s_attributes = graphics_text_attributes_create();
  
  tick_timer_service_subscribe(MINUTE_UNIT | DAY_UNIT, tick_handler);
  window_stack_push(my_window, false);
  layer_mark_dirty(draw_layer);
}


void handle_deinit(void) {
  app_message_deregister_callbacks(); 
  layer_destroy(draw_layer);
  layer_destroy(sun_layer);
  tick_timer_service_unsubscribe();
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
