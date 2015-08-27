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
static TextLayer *minute_layer;
static GRect minuteFrame = {.origin = {.x = 28 + 38, .y = 58}, .size = {.w = 144, .h = 50}};
static TextLayer *second_layer;
static GRect secondFrame = {.origin = {.x = 28 + 38, .y = 78}, .size = {.w = 144, .h = 50}};
static GFont s_hour_font;
static GFont s_minute_font;
static GFont s_second_font;
static GFont s_text_font;
static TextLayer *s_weather_layer;
static GRect weatherFrame;

enum {
  KEY_TEMPERATURE = 0,
  KEY_CONDITIONS = 1
};



static void main_window_load(Window *window) {
  
  s_hour_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DARK_40));
  s_minute_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_MONOMM_20));
  s_second_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_MONOMM_20));
  s_text_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_LIGHT_12));
  
  posInfo.X = 28;
  posInfo.HOUR_GAP = 38;
  posInfo.MIDDLE_GAP = 40;
  posInfo.MINUTE_GAP = 26;
  
  //Hour Layer
  hour_layer = text_layer_create(GRect(posInfo.X, 58, 144, 50));
  text_layer_set_background_color(hour_layer, GColorClear);
  text_layer_set_text_color(hour_layer, GColorBlack);
  // Improve the layout to be more like a watchface
  text_layer_set_font(hour_layer, s_hour_font);
  layer_add_child(window_get_root_layer(s_main_window),text_layer_get_layer(hour_layer));
  
  //Minute Layer
  minute_layer = text_layer_create(minuteFrame);
  text_layer_set_background_color(minute_layer, GColorClear);
  text_layer_set_text_color(minute_layer, GColorBlack);
  // Improve the layout to be more like a watchface
  text_layer_set_font(minute_layer, s_minute_font);
  layer_add_child(window_get_root_layer(s_main_window),text_layer_get_layer(minute_layer));
  
  //Second Layer
  second_layer = text_layer_create(secondFrame);
  text_layer_set_background_color(second_layer, GColorClear);
  text_layer_set_text_color(second_layer, GColorBlack);
  // Improve the layout to be more like a watchface
  text_layer_set_font(second_layer, s_second_font);
  layer_add_child(window_get_root_layer(s_main_window),text_layer_get_layer(second_layer));
  
  //Weather Layer
  if(clock_is_24h_style() == true) {
      weatherFrame.origin.x = posInfo.X + posInfo.HOUR_GAP + posInfo.MIDDLE_GAP;
      weatherFrame.origin.y = 61;
      s_weather_layer = text_layer_create(GRect(posInfo.X + posInfo.HOUR_GAP + posInfo.MIDDLE_GAP, 61, 144, 50));
  }
  else{
     weatherFrame.origin.x = posInfo.X + posInfo.HOUR_GAP + posInfo.MINUTE_GAP;
      weatherFrame.origin.y = 61;
      s_weather_layer = text_layer_create(weatherFrame);
  }
  text_layer_set_background_color(s_weather_layer, GColorClear);
  text_layer_set_text_color(s_weather_layer, GColorBlack);
  text_layer_set_text(s_weather_layer, "Loading...");
  text_layer_set_font(s_weather_layer, s_text_font);
  layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(s_weather_layer));

}

static void update_positions(){
  Layer *minuteLayer_layer = text_layer_get_layer(minute_layer);
  Layer *secondLayer_layer = text_layer_get_layer(second_layer);
  Layer *weatherLayer_layer = text_layer_get_layer(s_weather_layer);
  GSize hourSize = text_layer_get_content_size(hour_layer);
  GSize minuteSize = text_layer_get_content_size(minute_layer);
  GSize secondSize= text_layer_get_content_size(second_layer);
  
  posInfo.HOUR_GAP = hourSize.w + 1;
  printf("hour : %d : %d", hourSize.h, hourSize.w);
  printf("minute: %d : %d", minuteSize.h, minuteSize.w);
  printf("secondSize: %d : %d", secondSize.h, secondSize.w);
  if(minuteSize.w > secondSize.w) {
    posInfo.MIDDLE_GAP = minuteSize.w + 1;
  }
  else{
    posInfo.MIDDLE_GAP = secondSize.w + 1;
  }
  
  secondFrame.origin.x = minuteFrame.origin.x = posInfo.X + posInfo.HOUR_GAP;
  weatherFrame.origin.x = posInfo.X + posInfo.HOUR_GAP + posInfo.MIDDLE_GAP;
  
  layer_set_frame(minuteLayer_layer, minuteFrame);
  layer_set_frame(secondLayer_layer, secondFrame);
  layer_set_frame(weatherLayer_layer, weatherFrame);
  layer_mark_dirty(minuteLayer_layer);
  layer_mark_dirty(secondLayer_layer);
  layer_mark_dirty(weatherLayer_layer);

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
  
  update_time();
  update_positions();
  
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
  
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {

  // Store incoming information
  static char temperature_buffer[8];
  static char conditions_buffer[32];
  static char weather_layer_buffer[32];
  static char monthBuffer[] = "JUN\n";
  static char dateBuffer[] = "12\n";
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
  
  // Assemble full string and display
//     strftime(dateBuffer, sizeof("JUN\n12\n"), "%b\n%d\n", tick_time);
  strftime(monthBuffer, sizeof("SUN1225C"), "%^b", tick_time);
  strftime(dateBuffer, sizeof("SUN"), "%d", tick_time);
  snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s", temperature_buffer);
//   strcat(monthBuffer, "");
  strcat(monthBuffer, dateBuffer);
//   strcat(monthBuffer, "");
//     strcat(monthBuffer,weather_layer_buffer);
  strcat(weather_layer_buffer,"\n");
      strcat(weather_layer_buffer,monthBuffer);

    text_layer_set_text(s_weather_layer, weather_layer_buffer);
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
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  
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