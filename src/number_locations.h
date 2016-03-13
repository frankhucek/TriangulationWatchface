#include <pebble.h>

// pebble time 144x168
// Idea -> for each month that it is, take the place of that number to display the date

static const GRect frames[] =
{
  // GRect { origin :: GPoint, size :: GSize }
  // 12 
  {
    .origin = {.x = 65, .y = 0},
    .size = {.w = 15, .h = 15}
  },
  // 1
  {
    .origin = {.x = 97, .y = 15},
    .size = {.w = 15, .h = 15}
  },
  // 2
  {
    .origin = {.x = 123, .y = 40},
    .size = {.w = 15, .h = 15}
  },
  // 3
  {
    .origin = {.x = 129, .y = 76}, // y = 84
    .size = {.w = 15, .h = 15}
		/*.origin = {.x = 70, .y = 76},
		.size = {.w = 98, .h = 15}*/ // for date
  },
  // 4
  {
    .origin = {.x = 123, .y = 109},
    .size = {.w = 15, .h = 15}
  },
  // 5
  {
    .origin = {.x = 98, .y = 135},
    .size = {.w = 15, .h = 15}
  },
  // 6
  {
    .origin = {.x = 65, .y = 153},
    .size = {.w = 15, .h = 15}
  },
  // 7
  {
    .origin = {.x = 30, .y = 135},
    .size = {.w = 15, .h = 15}
  },
  // 8
  {
    .origin = {.x = 9, .y = 109},
    .size = {.w = 15, .h = 15}
  },
  // 9
  {
    .origin = {.x = 0, .y = 76},
    .size = {.w = 15, .h = 15}
  },
  // 10
  {
    .origin = {.x = 10, .y = 43},
    .size = {.w = 15, .h = 15}
  },
  // 11
  {
    .origin = {.x = 34, .y = 15},
    .size = {.w = 15, .h = 15}
  }
};