#include <pebble.h>
#include <stdio.h>
#include "number_locations.h"

Window *window;
Layer *hands_layer;
TextLayer *bg_layer;
TextLayer *numbers[12];
//GPath *hour_hand, *minute_hand, *second_hand;

void init(void);
void deinit(void);
void window_load(Window *w);
void window_unload(Window *w);
void draw_numbers(Layer *parent);
void handle_tick(struct tm *tick_time, TimeUnits units_changed);
static void hands_update_proc(Layer *layer, GContext *ctx);
char *itos(int i);

int main(void)
{
  init();
  app_event_loop();
  deinit();
  return 0;
}

void init(void)
{
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    // handle update procs in window_load
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, true);
  
  tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
}

// subscribed to be called each second
void handle_tick(struct tm *tick_time, TimeUnits units_changed)
{
  // calls update_procedure of all child layers of window
  layer_mark_dirty(window_get_root_layer(window));
}

void window_load(Window *w)
{
  Layer *w_layer = window_get_root_layer(w);
  GRect w_bounds = layer_get_bounds(w_layer);
  
  bg_layer = text_layer_create(w_bounds);
  text_layer_set_background_color(bg_layer, GColorBlack);
  text_layer_set_text_color(bg_layer, GColorDarkCandyAppleRed);
  layer_add_child(w_layer, text_layer_get_layer(bg_layer));
  
  // draw numbers on screen
  draw_numbers(w_layer);
  
  hands_layer = layer_create(w_bounds);
  layer_set_update_proc(hands_layer, hands_update_proc);
  layer_add_child(w_layer, hands_layer);
}

// 
void draw_numbers(Layer *parent)
{
  int i;
  for(i = 0; i < 12; i++)
  {
    numbers[i] = text_layer_create(frames[i]);
    text_layer_set_background_color(numbers[i], GColorBlack);
    text_layer_set_text_color(numbers[i], GColorWhite);
    text_layer_set_text_alignment(numbers[i], GTextAlignmentCenter);
    text_layer_set_text(numbers[i], itos(i));
    layer_add_child(parent, text_layer_get_layer(numbers[i]));
  }
}

// because pebble string libraries don't exist...
char *itos(int i)
{
  char *c;
  switch(i)
  {
    case 0:
      c = "0";
      break;
    case 1:
      c = "1";
      break;
    case 2:
      c = "2";
      break;
    case 3:
      c = "3";
      break;
    case 4:
      c = "4";
      break;
    case 5:
      c = "5";
      break;
    case 6:
      c = "6";
      break;
    case 7:
      c = "7";
      break;
    case 8:
      c = "8";
      break;
    case 9:
      c = "9";
      break;
    case 10:
      c = "10";
      break;
    case 11:
      c = "11";
      break;
    default:
      c = "unkown input";
      break;
  }
  return c;
}

// custom drawing happens here
// passing in hands_layer and its GContext
static void hands_update_proc(Layer *layer, GContext *ctx)
{
  GRect bounds = layer_get_bounds(layer);
  GPoint center = grect_center_point(&bounds);
  
  // get time
  time_t tme = time(NULL);
  struct tm *t = localtime(&tme);
  
  int32_t hour_length = (bounds.size.w / 3) - 8; // A third the width of the hands_layer bounds
  int32_t minute_length = (bounds.size.w / 2) - 12; // Half the width of the hands_layer bounds
  int32_t second_length = (bounds.size.w / 2) - 12;
  
  // get angle from center of all 3 points
  int32_t sec_angle = TRIG_MAX_ANGLE * t->tm_sec / 60;
  int32_t min_angle = TRIG_MAX_ANGLE * t->tm_min / 60; 
  //int32_t hour_angle = TRIG_MAX_ANGLE * (((t->tm_hour % 12) * 6) + (t->tm_min / 10)) / (12 * 6);
  // easier to understand with this format of clock
  int32_t hour_angle = TRIG_MAX_ANGLE * ((t->tm_hour % 12) * 6) / (12 * 6);
  
  // create 3 points. hour, minute, second
  GPoint hour = 
  {
    .x = (int16_t)(sin_lookup(hour_angle) * (int32_t)hour_length / TRIG_MAX_RATIO) + center.x,
    .y = (int16_t)(-cos_lookup(hour_angle) * (int32_t)hour_length / TRIG_MAX_RATIO) + center.y,
  };
  
  GPoint minute =
  {
    .x = (int16_t)(sin_lookup(min_angle) * (int32_t)minute_length / TRIG_MAX_RATIO) + center.x,
    .y = (int16_t)(-cos_lookup(min_angle) * (int32_t)minute_length / TRIG_MAX_RATIO) + center.y,
  };
  
  GPoint second =
  {
    .x = (int16_t)(sin_lookup(sec_angle) * (int32_t)second_length / TRIG_MAX_RATIO) + center.x,
    .y = (int16_t)(-cos_lookup(sec_angle) * (int32_t)second_length / TRIG_MAX_RATIO) + center.y,
  };
  
  // connect points/draw lines, fill colors
  graphics_context_set_stroke_width(ctx, 3);
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_context_set_stroke_color(ctx, GColorWhite);
  
  graphics_draw_line(ctx, minute, second);
  graphics_draw_line(ctx, second, hour);
  
  graphics_context_set_fill_color(ctx, GColorBlue);
  graphics_context_set_stroke_color(ctx, GColorBlue);
  graphics_draw_line(ctx, hour, minute);
	//graphics_draw_line(ctx, hour, center);
	//graphics_draw_line(ctx, minute,center);
}

void window_unload(Window *w)
{
  layer_destroy(hands_layer);
}

void deinit(void)
{
  tick_timer_service_unsubscribe();
  window_destroy(window);  
}