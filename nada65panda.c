/* Copyright 2022 MIIIW <www.miiiw.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "nada65panda.h"

#include "i2c_master.h"

#define LOOP_10HZ_PERIOD 100

extern void rgb_matrix_update_pwm_buffers(void);

uint8_t side_cmd[9] = {0x5a, 0x12, 0xff, 0xff, 0xff, 0x40, 0x80, 0x0, 0x0};
uint8_t default_side_cmd[9] = {0x5a, 0x12, 0xff, 0xff, 0xff,
                               0x40, 0x80, 0x0,  0x0};

deferred_token loop10hz_token = INVALID_DEFERRED_TOKEN;

static uint32_t rgb_press_timer = 0;
static uint32_t rgb_blink_timer = 0;

static uint8_t rgb_color = 0;

uint32_t loop_10Hz(uint32_t trigger_time, void *cb_arg);

typedef union {
  uint32_t raw;
  struct {
    uint8_t rgb_color : 8;
    uint8_t side_rgb_mode : 8;
    uint8_t side_rgb_color : 8;
    uint8_t side_rgb_value : 8;
    uint8_t side_rgb_speed : 8;
  };
} user_config_t;

user_config_t user_config;

void eeconfig_init_kb(void) {
  user_config.raw = 0;
  user_config.rgb_color = 0;
  user_config.side_rgb_mode = 0x0;
  user_config.side_rgb_color = 0x0;
  user_config.side_rgb_value = 0x2;
  user_config.side_rgb_speed = 0x2;
  eeconfig_update_user(user_config.raw);
}

void keyboard_pre_init_kb() {
  keyboard_pre_init_user();
  i2c_init();
}

void keyboard_post_init_kb(void) {
  keyboard_post_init_user();

  user_config.raw = eeconfig_read_user();
  rgb_color = user_config.rgb_color + 1;

  uint8_t brightcmd;
  brightcmd = 28; // KB351:21 led_cnt=1;28:led_cnt=8
  i2c_transmit(0x8a, &brightcmd, 1, 100);

  switch (user_config.side_rgb_mode) {
  case 1: // off
    side_cmd[1] = 0x0;
    break;
  case 2: // single_color
    side_cmd[1] = 0x1;
    break;
  case 3: // single_breath
    side_cmd[1] = 0x3;
    break;
  case 0: // colorful_rainbow
  default:
    side_cmd[1] = 0x12;
  }

  switch (user_config.side_rgb_color) {
  case 1: // green
    side_cmd[2] = 0;
    side_cmd[3] = 0xff;
    side_cmd[4] = 0;
    break;
  case 2: //
    side_cmd[2] = 0;
    side_cmd[3] = 0;
    side_cmd[4] = 0xff;
    break;
  case 3: //
    side_cmd[2] = 0xff;
    side_cmd[3] = 0xff;
    side_cmd[4] = 0;
    break;
  case 4: //
    side_cmd[2] = 0xff;
    side_cmd[3] = 0;
    side_cmd[4] = 0xff;
    break;
  case 5: //
    side_cmd[2] = 0;
    side_cmd[3] = 0xff;
    side_cmd[4] = 0xff;
    break;
  case 6: //
    side_cmd[2] = 0xff;
    side_cmd[3] = 0xff;
    side_cmd[4] = 0xff;
    break;
  case 7:
    if (side_cmd[1] == 0x1) {
      side_cmd[1] = 0x13;
    }
    side_cmd[2] = 0xff;
    side_cmd[3] = 0;
    side_cmd[4] = 0xff;
    break;
  case 0: // colorful_rainbow
  default:
    side_cmd[2] = 0xff;
    side_cmd[3] = 0;
    side_cmd[4] = 0;
  }

  side_cmd[5] = 0x40 / 4 * (user_config.side_rgb_value);
  side_cmd[6] = 0xff / 5 * (user_config.side_rgb_speed + 1);
  i2c_transmit(0x8a, side_cmd, 9, 100);

  loop10hz_token = defer_exec(LOOP_10HZ_PERIOD, loop_10Hz, NULL);
}

uint8_t logo_led_mode;

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
  if (!process_record_user(keycode, record)) {
    return false;
  }

  switch (keycode) {
  case RGB_RST:
    if (record->event.pressed) {
      rgb_press_timer = timer_read32();
    } else
      rgb_press_timer = 0;
    return false;
  case RGB_COLOR:
    if (record->event.pressed) {
      uint8_t rgb_value;
      rgb_value = rgb_matrix_get_val();
      switch (rgb_color) {
      case 1:
        rgb_matrix_sethsv(21, 255, rgb_value); // rgb_matrix_sethsv(HSV_ORANGE);
        break;
      case 2:
        rgb_matrix_sethsv(43, 255, rgb_value); // rgb_matrix_sethsv(HSV_YELLOW);
        break;
      case 3:
        rgb_matrix_sethsv(85, 255, rgb_value); // rgb_matrix_sethsv(HSV_GREEN);
        break;
      case 4:
        rgb_matrix_sethsv(128, 255, rgb_value); // rgb_matrix_sethsv(HSV_CYAN);
        break;
      case 5:
        rgb_matrix_sethsv(170, 255, rgb_value); // rgb_matrix_sethsv(HSV_BLUE);
        break;
      case 6:
        rgb_matrix_sethsv(191, 255,
                          rgb_value); // rgb_matrix_sethsv(HSV_PURPLE);
        break;
      default:
        rgb_color = 0;
        rgb_matrix_sethsv(0, 255, rgb_value); // rgb_matrix_sethsv(HSV_RED);
      }
      user_config.rgb_color = rgb_color;
      eeconfig_update_user(user_config.raw);
      rgb_color++;
    }
    return false;
  case S_MODE:
    if (record->event.pressed) {
      user_config.side_rgb_mode++;
      if (user_config.side_rgb_mode >= 4)
        user_config.side_rgb_mode = 0;
      eeconfig_update_user(user_config.raw);
      switch (user_config.side_rgb_mode) {
      case 1: // off
        side_cmd[1] = 0x0;
        break;
      case 2: // single_color
        side_cmd[1] = 0x1;
        break;
      case 3: // single_breath
        side_cmd[1] = 0x3;
        break;
      case 0: // colorful_rainbow
      default:
        side_cmd[1] = 0x12;
      }
      i2c_transmit(0x8a, side_cmd, 9, 100);
    }
    return false;
  case S_COLOR:
    if (record->event.pressed) {
      user_config.side_rgb_color++;
      if (user_config.side_rgb_color >= 8)
        user_config.side_rgb_color = 0;
      eeconfig_update_user(user_config.raw);
      if (side_cmd[1] == 0x13)
        side_cmd[1] = 0x1;
      switch (user_config.side_rgb_color) {
      case 1: // green
        side_cmd[2] = 0;
        side_cmd[3] = 0xff;
        side_cmd[4] = 0;
        break;
      case 2: //
        side_cmd[2] = 0;
        side_cmd[3] = 0;
        side_cmd[4] = 0xff;
        break;
      case 3: //
        side_cmd[2] = 0xff;
        side_cmd[3] = 0xff;
        side_cmd[4] = 0;
        break;
      case 4: //
        side_cmd[2] = 0xff;
        side_cmd[3] = 0;
        side_cmd[4] = 0xff;
        break;
      case 5: //
        side_cmd[2] = 0;
        side_cmd[3] = 0xff;
        side_cmd[4] = 0xff;
        break;
      case 6: //
        side_cmd[2] = 0xff;
        side_cmd[3] = 0xff;
        side_cmd[4] = 0xff;
        break;
      case 7:
        if (side_cmd[1] == 0x1) {
          side_cmd[1] = 0x13;
          side_cmd[2] = 0xff;
          side_cmd[3] = 0;
          side_cmd[4] = 0xff;
        }
        side_cmd[2] = 0xff;
        side_cmd[3] = 0;
        side_cmd[4] = 0xff;
        break;
      case 0: // red
      default:
        side_cmd[2] = 0xff;
        side_cmd[3] = 0;
        side_cmd[4] = 0;
      }
      i2c_transmit(0x8a, side_cmd, 9, 100);
    }
    return false;
  case S_SPEED:
    if (record->event.pressed) {
      user_config.side_rgb_speed++;
      if (user_config.side_rgb_speed >= 4)
        user_config.side_rgb_speed = 0;
      eeconfig_update_user(user_config.raw);
      side_cmd[6] = 0xff / 5 * (user_config.side_rgb_speed + 1);
      i2c_transmit(0x8a, side_cmd, 9, 100);
    }
    return false;
  case S_VALUE:
    if (record->event.pressed) {
      user_config.side_rgb_value++;
      if (user_config.side_rgb_value > 4)
        user_config.side_rgb_value = 0;
      eeconfig_update_user(user_config.raw);
      side_cmd[5] = 0x40 / 4 * (user_config.side_rgb_value);
      i2c_transmit(0x8a, side_cmd, 9, 100);
    }
    return false;
  }
  return true;
}

uint32_t loop_10Hz(uint32_t trigger_time, void *cb_arg) {
  extend_deferred_exec(loop10hz_token, LOOP_10HZ_PERIOD);

  if (rgb_press_timer && timer_elapsed32(rgb_press_timer) > 3000) {
    rgb_press_timer = 0;

    uint8_t i;

    // power on first to avoid RGB_TOGGLE_OFF
    setPinOutput(A8);
    writePinHigh(A8);
    writePinHigh(RGB_EN_PIN);
    // power on first to avoid RGB_TOGGLE_OFF

    for (i = 0; i < 3; i++) {
      rgb_blink_timer = timer_read32();
      while (timer_elapsed32(rgb_blink_timer) < 500) {
        rgb_matrix_set_color_all(200, 200, 200);
        rgb_matrix_update_pwm_buffers();
        wait_ms(10);
      }
      rgb_blink_timer = timer_read32();
      while (timer_elapsed32(rgb_blink_timer) < 500) {
        rgb_matrix_set_color_all(RGB_OFF);
        rgb_matrix_update_pwm_buffers();
        wait_ms(10);
      }
    }

    logo_led_mode = 0xff;

    rgb_matrix_mode(RGB_MATRIX_DEFAULT_MODE);
    rgb_matrix_set_speed(RGB_MATRIX_DEFAULT_SPD);
    rgb_matrix_sethsv(RGB_MATRIX_DEFAULT_HUE, RGB_MATRIX_DEFAULT_SAT,
                      RGB_MATRIX_DEFAULT_VAL);
    ;
    rgb_matrix_enable();
  }

  static uint32_t debug_tick = 0;

  if (debug_tick++ > 19) {
    dprintf("trigger %ld\n", trigger_time);
    debug_tick = 0;
  }

  return LOOP_10HZ_PERIOD;
}

#ifdef RGB_MATRIX_ENABLE
#define __ NO_LED
led_config_t g_led_config = {
    {
        // Key Matrix to LED Index
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
        {29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15},
        {30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, __, 42, 43},
        {57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, __, 46, 45, 44},
        {58, 59, 60, __, __, __, 61, __, __, 62, 63, __, 64, 65, 66},
    },
    {// LED Index to Physical Position
     {0, 0},    {15, 0},   {29, 0},   {44, 0},   {59, 0},   {73, 0},
     {88, 0},   {103, 0},  {118, 0},  {132, 0},  {147, 0},  {162, 0},
     {176, 0},  {198, 0},  {224, 0},  {224, 15}, {202, 15}, {184, 15},
     {169, 15}, {154, 15}, {140, 15}, {125, 15}, {110, 15}, {95, 15},
     {81, 15},  {66, 15},  {51, 15},  {37, 15},  {22, 15},  {4, 15},
     {6, 30},   {26, 30},  {40, 30},  {55, 30},  {70, 30},  {84, 30},
     {99, 30},  {114, 30}, {129, 30}, {143, 30}, {158, 30}, {173, 30},
     {196, 30}, {224, 30}, {224, 45}, {209, 49}, {185, 45}, {165, 45},
     {151, 45}, {136, 45}, {121, 45}, {106, 45}, {92, 45},  {77, 45},
     {62, 45},  {48, 45},  {33, 45},  {9, 45},   {2, 60},   {20, 60},
     {39, 60},  {94, 60},  {152, 60}, {170, 60}, {195, 64}, {209, 64},
     {224, 64}},
    {// RGB LED Index to Flag
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 4, 4, 4,
     4, 4, 4, 4, 4, 1, 1, 1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 1, 4,
     4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1}};

#endif // RGB_MATRIX_ENABLE
