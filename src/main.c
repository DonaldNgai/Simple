#include <pebble.h>

  struct positionInfo {
  int X;
  int HOUR_GAP;
  int MIDDLE_GAP;
  int MINUTE_GAP;
};

static struct positionInfo posInfo;
  
static Window *s_main_window;
static TextLayer *hour_layer;
static GRect hourFrame = {.origin = {.x = 15, .y = 58}, .size = {.w = 140, .h = 50}};
static TextLayer *minute_layer;
static GRect minuteFrame = {.origin = {.x = 28 + 38, .y = 58}, .size = {.w = 40, .h = 22}};
static TextLayer *second_layer;
static GRect secondFrame = {.origin = {.x = 28 + 38, .y = 78}, .size = {.w = 40, .h = 22}};
static GFont s_hour_font;
static GFont s_minute_font;
static GFont s_second_font;
static GFont s_text_font;
static TextLayer *s_weather_layer;
static GRect weatherFrame = {.origin = {.x = 0, .y = 60}, .size = {.w = 40, .h = 50}};
static char weatherLongBuf[10];

enum {
  KEY_TEMPERATURE = 0,
  KEY_CONDITIONS = 1
};



static void main_window_load(Window *window) {
  
  s_hour_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DARK_40));
  s_minute_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_MONOMM_20));
  s_second_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_MONOMM_20));
  s_text_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_LIGHT_12));
  
  posInfo.X = 15;
  posInfo.HOUR_GAP = 38;
  posInfo.MIDDLE_GAP = 40;
  posInfo.MINUTE_GAP = 26;
  
  window_set_background_color(s_main_window, GColorBlack);

  //Hour Layer
  hour_layer = text_layer_create(hourFrame);
  text_layer_set_background_color(hour_layer, GColorClear);
  text_layer_set_text_color(hour_layer, GColorWhite);
  // Improve the layout to be more like a watchface
  text_layer_set_font(hour_layer, s_hour_font);
  text_layer_set_text(hour_layer, "00");
  layer_add_child(window_get_root_layer(s_main_window),text_layer_get_layer(hour_layer));
  
  //Minute Layer
  minute_layer = text_layer_create(minuteFrame);
  text_layer_set_background_color(minute_layer, GColorClear);
  text_layer_set_text_color(minute_layer, GColorWhite);
  // Improve the layout to be more like a watchface
  text_layer_set_font(minute_layer, s_minute_font);
  text_layer_set_text(minute_layer, "00");
  layer_add_child(window_get_root_layer(s_main_window),text_layer_get_layer(minute_layer));
  
  //Second Layer
  second_layer = text_layer_create(secondFrame);
  text_layer_set_background_color(second_layer, GColorClear);
  text_layer_set_text_color(second_layer, GColorWhite);
  // Improve the layout to be more like a watchface
  text_layer_set_font(second_layer, s_second_font);
  layer_add_child(window_get_root_layer(s_main_window),text_layer_get_layer(second_layer));
  
  //Weather Layer
  if(clock_is_24h_style() == true) {
      weatherFrame.origin.x = posInfo.X + posInfo.HOUR_GAP + posInfo.MIDDLE_GAP;
      weatherFrame.origin.y = 61;
      s_weather_layer = text_layer_create(weatherFrame);
  }
  else{
      weatherFrame.origin.x = posInfo.X + posInfo.HOUR_GAP + posInfo.MINUTE_GAP;
      weatherFrame.origin.y = 61;
      s_weather_layer = text_layer_create(weatherFrame);
  }
  text_layer_set_background_color(s_weather_layer, GColorBlack);
  text_layer_set_text_color(s_weather_layer, GColorWhite);
  text_layer_set_text(s_weather_layer, "Loading...");
  text_layer_set_font(s_weather_layer, s_text_font);
  layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(s_weather_layer));

}

static void update_positions(){
    printf("update position");
      printf("y's' before h:%d m:%d s:%d w:%d", hourFrame.origin.y, minuteFrame.origin.y, secondFrame.origin.y, weatherFrame.origin.y);

  Layer *window_layer = window_get_root_layer(s_main_window);
  Layer *hourLayer_layer = text_layer_get_layer(hour_layer);
  Layer *minuteLayer_layer = text_layer_get_layer(minute_layer);
  Layer *secondLayer_layer = text_layer_get_layer(second_layer);
  Layer *weatherLayer_layer = text_layer_get_layer(s_weather_layer);
  GSize hourSize = text_layer_get_content_size(hour_layer);
  GSize minuteSize = text_layer_get_content_size(minute_layer);
  GSize secondSize= text_layer_get_content_size(second_layer);
  GSize weatherSize= text_layer_get_content_size(s_weather_layer);
  
    posInfo.HOUR_GAP = hourSize.w + 2;
//   printf("hour : %d : %d", hourSize.h, hourSize.w);
//   printf("minute: %d : %d", minuteSize.h, minuteSize.w);
//   printf("secondSize: %d : %d", secondSize.h, secondSize.w);
  if(minuteSize.w > secondSize.w) {
    posInfo.MIDDLE_GAP = minuteSize.w + 1;
  }
  else{
    posInfo.MIDDLE_GAP = secondSize.w + 1;
  }
  
  GRect bounds = layer_get_bounds(window_layer);
  printf("window size: %d sub: %d add: %d", bounds.size.h, (bounds.size.h/2) - (hourSize.h/2), (bounds.size.h/2) + (hourSize.h/2));
  posInfo.X = (bounds.size.w - (posInfo.HOUR_GAP + posInfo.MIDDLE_GAP + weatherSize.w))/2;
  
  hourFrame.origin.y = minuteFrame.origin.y = (bounds.size.h/2) - (hourSize.h/2);
  weatherFrame.origin.y =hourFrame.origin.y + 3;
  secondFrame.origin.y = minuteFrame.origin.y + minuteSize.h + 1;
  
  hourFrame.origin.x = posInfo.X;
  secondFrame.origin.x = minuteFrame.origin.x = posInfo.X + posInfo.HOUR_GAP;
  weatherFrame.origin.x = posInfo.X + posInfo.HOUR_GAP + posInfo.MIDDLE_GAP;
        printf("y's' middle h:%d m:%d s:%d w:%d", hourFrame.origin.y, minuteFrame.origin.y, secondFrame.origin.y, weatherFrame.origin.y);

  layer_set_frame(hourLayer_layer, hourFrame);
  layer_set_frame(minuteLayer_layer, minuteFrame);
  layer_set_frame(secondLayer_layer, secondFrame);
  layer_set_frame(weatherLayer_layer, weatherFrame);
  layer_mark_dirty(hourLayer_layer);
  layer_mark_dirty(minuteLayer_layer);
  layer_mark_dirty(secondLayer_layer);
  layer_mark_dirty(weatherLayer_layer);
      printf("y's' after h:%d m:%d s:%d w:%d", hourFrame.origin.y, minuteFrame.origin.y, secondFrame.origin.y, weatherFrame.origin.y);

}

static void main_window_unload(Window *window) {
  
  fonts_unload_custom_font(s_hour_font);
  fonts_unload_custom_font(s_text_font);
  text_layer_destroy(s_weather_layer);
  text_layer_destroy(hour_layer);
  text_layer_destroy(minute_layer);
  text_layer_destroy(second_layer);
  
}

static void update_time(){
//     printf("update time");

  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  static char hourBuffer[] = "00";
  static char minuteBuffer[] = "00";
  static char secondBuffer[] = "AM";
  
   // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(hourBuffer, sizeof("00"), "%I", tick_time);
    strftime(minuteBuffer, sizeof("00"), "%M", tick_time);
    strftime(secondBuffer, sizeof("SUN"), "%a", tick_time);
  } else {
    // Use 12 hour format
    strftime(hourBuffer, sizeof("00"), "%I", tick_time);
    strftime(minuteBuffer, sizeof("00"), "%M", tick_time);
    strftime(secondBuffer, sizeof("AM"), "%p", tick_time);
}
  
  // Display this time on the TextLayer
  text_layer_set_text(hour_layer, hourBuffer);
  text_layer_set_text(minute_layer, minuteBuffer);
  text_layer_set_text(second_layer, secondBuffer);
  
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
//   printf("tick");
//   printf("buf bef: %s",weatherLongBuf);
//       printf("weatherlayer before %s", text_layer_get_text(s_weather_layer));

  update_positions();
//     printf("weatherlayer middle %s", text_layer_get_text(s_weather_layer));

  // Get weather update every 30 minutes
  if(tick_time->tm_min % 30 == 0) {
    // Begin dictionary
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
  
    // Add a key-value pair
    dict_write_uint8(iter, 0, 0);
  
    // Send the message!
    app_message_outbox_send();
  }
  
  update_time();
  text_layer_set_text(s_weather_layer, weatherLongBuf);
//   printf("weatherlayer after %s", text_layer_get_text(s_weather_layer));
//   printf("buf aft: %s",weatherLongBuf);

}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
        printf("y's' beforesaved1 h:%d m:%d s:%d w:%d", hourFrame.origin.y, minuteFrame.origin.y, secondFrame.origin.y, weatherFrame.origin.y);

  // Store incoming information
  char temperature_buffer[8];
  char conditions_buffer[32];
  char weather_layer_buffer[32];
  char dateBuffer[] = "12\n";

  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL) {
    
    // Which key was received?
    switch(t->key) {
      case KEY_TEMPERATURE:
        snprintf(temperature_buffer, sizeof(temperature_buffer), "%dC", (int)t->value->int32);
        break;
        
      case KEY_CONDITIONS:
        snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", t->value->cstring);
        break;
        
      default:
        APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
        break;
    }

    // Look for next item

    t = dict_read_next(iterator);
  }
          printf("y's' beforesaved1 h:%d m:%d s:%d w:%d", hourFrame.origin.y, minuteFrame.origin.y, secondFrame.origin.y, weatherFrame.origin.y);

  // Assemble full string and display
    strftime(dateBuffer, sizeof("JUN1225C1111"), "%b\n%d\n", tick_time);
          printf("y's' middlesaved1 h:%d m:%d s:%d w:%d", hourFrame.origin.y, minuteFrame.origin.y, secondFrame.origin.y, weatherFrame.origin.y);

  snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s", temperature_buffer);
//   printf("weatherlayer before %s", text_layer_get_text(s_weather_layer));
//       printf("dateBuffer before %s", dateBuffer);
                    printf("y's' aftersaved1 h:%d m:%d s:%d w:%d", hourFrame.origin.y, minuteFrame.origin.y, secondFrame.origin.y, weatherFrame.origin.y);

      strcat(dateBuffer,weather_layer_buffer);
//   printf("dateBuffer after %s", dateBuffer);
              printf("y's' aftersaved2 h:%d m:%d s:%d w:%d", hourFrame.origin.y, minuteFrame.origin.y, secondFrame.origin.y, weatherFrame.origin.y);

  strcpy( weatherLongBuf, dateBuffer );
                printf("y's' aftersaved3 h:%d m:%d s:%d w:%d", hourFrame.origin.y, minuteFrame.origin.y, secondFrame.origin.y, weatherFrame.origin.y);

  text_layer_set_text(s_weather_layer, weatherLongBuf);
//   printf("saved: %s",weatherLongBuf);
  
        printf("y's' beforesaved h:%d m:%d s:%d w:%d", hourFrame.origin.y, minuteFrame.origin.y, secondFrame.origin.y, weatherFrame.origin.y);
//   update_positions();
          printf("y's' middlesaved h:%d m:%d s:%d w:%d", hourFrame.origin.y, minuteFrame.origin.y, secondFrame.origin.y, weatherFrame.origin.y);

  update_time();
          printf("y's' aftersaved h:%d m:%d s:%d w:%d", hourFrame.origin.y, minuteFrame.origin.y, secondFrame.origin.y, weatherFrame.origin.y);

  }

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void init() {

   // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  //Prepare AppMessage
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  update_time();
  
}

static void deinit() {
  
  window_destroy(s_main_window);

}

int main(void) {
  init();
  app_event_loop();
  deinit();
  return 0;
}