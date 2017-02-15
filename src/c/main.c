#include <pebble.h>
  
static Window *s_main_window;
static TextLayer *s_time_layer, *s_time_date;
static GFont *s_time_font_josefin_60, *s_time_font_josefin_19, *s_time_font_josefin_69, *s_time_font_josefin_55;

static void update_time() {
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  
  static char time[20];
  static char date[20];

  strftime(date, sizeof(date), "%B %e", tick_time);
  
  
    strftime(time, sizeof("00:00"), "%I:%M", tick_time);
    if (time[0] == '0') {
 			memmove(&time[0], &time[1], sizeof(time) - 1); //remove leading zero
      text_layer_set_font(s_time_layer, s_time_font_josefin_69);
  		} 
    if(time[1] == '0' || time[1] == '2') {
        text_layer_set_font(s_time_layer, s_time_font_josefin_55);
        layer_set_frame(text_layer_get_layer(s_time_layer), GRect(0,46,144,90));
    }
    if(time[1] == '0' && time[3] == '1') {
        text_layer_set_font(s_time_layer, s_time_font_josefin_60);
        layer_set_frame(text_layer_get_layer(s_time_layer), GRect(0,42,144,90));
    }
    if(time[1] == '2' && time[3] == '1') {
        text_layer_set_font(s_time_layer, s_time_font_josefin_69);
        layer_set_frame(text_layer_get_layer(s_time_layer), GRect(0,32,144,90));
    }
    if(time[1] == '2' && time[3] == '2') {
        text_layer_set_font(s_time_layer, s_time_font_josefin_60);
        layer_set_frame(text_layer_get_layer(s_time_layer), GRect(0,42,144,90));
    }
  //else if (time[0] == '1') {
//       s_time_layer = text_layer_create(GRect(0,44,144,90));
//       text_layer_set_font(s_time_layer, s_time_font_josefin_55);
//       layer_set_frame(text_layer_get_layer(s_time_layer), GRect(0,46,144,90));
//     }

  text_layer_set_text(s_time_layer, time);
  text_layer_set_text(s_time_date, date);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {
  s_time_layer = text_layer_create(GRect(0,32,144,90));
  s_time_date = text_layer_create(GRect(0,101,144,90));
  
  s_time_font_josefin_55 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_JOSEFIN_57));
  s_time_font_josefin_60 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_JOSEFIN_60));
  s_time_font_josefin_19 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_JOSEFIN_19));
  s_time_font_josefin_69 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_JOSEFIN_69));
  
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_background_color(s_time_date, GColorClear);
  
  text_layer_set_text(s_time_layer, "");
  text_layer_set_text(s_time_date, "");
  
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text_color(s_time_date, GColorWhite);
  
  text_layer_set_font(s_time_layer, s_time_font_josefin_69);
  text_layer_set_font(s_time_date, s_time_font_josefin_19);
  
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_time_date, GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_date));
  
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_time_date);
  
  fonts_unload_custom_font(s_time_font_josefin_55);
  fonts_unload_custom_font(s_time_font_josefin_60);
  fonts_unload_custom_font(s_time_font_josefin_19);
  fonts_unload_custom_font(s_time_font_josefin_69);
}

static void init() {
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorDarkGray);
  
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  window_stack_push(s_main_window, true);
  
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  update_time();
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}