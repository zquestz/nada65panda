// Copyright 2020 QMK
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H

extern uint8_t device_mode;

bool km_win = true;
bool win_lock = false;
enum layers {
  WIN_BL,
  WIN_FN,
  MAC_BL,
  MAC_FN,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [WIN_BL] =
        LAYOUT(KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9,
               KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_DEL, KC_TAB, KC_Q, KC_W, KC_E,
               KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC,
               KC_BSLS, KC_HOME, KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H,
               KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT, KC_PGUP, KC_LSFT,
               KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT,
               KC_SLSH, KC_RSFT, KC_UP, KC_PGDN, KC_LCTL, KC_LWIN, KC_LALT,
               KC_SPC, KC_RCTL, MO(WIN_FN), KC_LEFT, KC_DOWN, KC_RGHT),

    [WIN_FN] = LAYOUT(
        KC_GRV, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9,
        KC_F10, KC_F11, KC_F12, RGB_MOD, KC_INS, _______, DEV_BT1, DEV_BT2,
        DEV_BT3, DEV_RF24, _______, _______, S_MODE, S_COLOR, S_VALUE, S_SPEED,
        KC_PSCR, KC_PAUS, RGB_TOG, KC_END, _______, _______, _______, _______,
        _______, _______, _______, _______, DEV_CLR, _______, RGB_COLOR,
        RGB_RST, _______, KC_PSCR, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, RGB_VAI,
        KC_PAUS, _______, W_LOCK, _______, _______, _______, _______, RGB_SPD,
        RGB_VAD, RGB_SPI),

    [MAC_BL] =
        LAYOUT(KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9,
               KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_DEL, KC_TAB, KC_Q, KC_W, KC_E,
               KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC,
               KC_BSLS, KC_HOME, KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H,
               KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT, KC_PGUP, KC_LSFT,
               KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT,
               KC_SLSH, KC_RSFT, KC_UP, KC_PGDN, KC_LCTL, KC_LOPT, KC_LCMD,
               KC_SPC, KC_RCTL, MO(MAC_FN), KC_LEFT, KC_DOWN, KC_RGHT),

    [MAC_FN] = LAYOUT(
        KC_GRV, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9,
        KC_F10, KC_F11, KC_F12, RGB_MOD, KC_INS, _______, DEV_BT1, DEV_BT2,
        DEV_BT3, DEV_RF24, _______, _______, S_MODE, S_COLOR, S_VALUE, S_SPEED,
        LSG(KC_3), KC_PAUS, RGB_TOG, KC_END, _______, _______, _______, _______,
        _______, _______, _______, _______, DEV_CLR, _______, RGB_COLOR,
        RGB_RST, _______, LSG(KC_3), _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______,
        RGB_VAI, KC_PAUS, _______, _______, _______, _______, _______, _______,
        RGB_SPD, RGB_VAD, RGB_SPI),

};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  case W_LOCK:
    if (record->event.pressed) {
      win_lock = !win_lock;
    }
    return false;
  case KC_LGUI:
    if (km_win) {
      if (win_lock)
        return false;
      else
        return true;
    }
    break;
  }
  return true;
}

bool rgb_matrix_indicators_user(void) {
  if (host_keyboard_led_state().caps_lock) {
    rgb_matrix_set_color(30, 200, 200, 200);
  } else {
    if (!rgb_matrix_get_flags()) {
      rgb_matrix_set_color(30, 0, 0, 0);
    }
  }

  if (win_lock && km_win) {
    rgb_matrix_set_color(59, 200, 200, 200);
  } else {
    if (!rgb_matrix_get_flags()) {
      rgb_matrix_set_color(59, 0, 0, 0);
    }
  }
  return true;
}

#ifdef DIP_SWITCH_ENABLE
bool dip_switch_update_user(uint8_t index, bool active) {
  switch (index) {
  case 0:
    default_layer_set(1UL << (active ? WIN_BL : MAC_BL));
    km_win = active;
    break;
  }
  return true;
}
#endif

layer_state_t default_layer_state_set_user(layer_state_t state) {
  uint8_t layer = get_highest_layer(state);
  if (layer == MAC_BL) {
    keymap_config.nkro = false;
    eeconfig_update_keymap(keymap_config.raw);
  } else {
    keymap_config.nkro = true;
    eeconfig_update_keymap(keymap_config.raw);
  }
  return state;
}
