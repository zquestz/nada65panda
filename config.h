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

#pragma once
/* RGB Matrix */
#ifdef RGB_MATRIX_ENABLE
#define RGB_MATRIX_KEYPRESSES // reacts to keypresses
#define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#define RGB_MATRIX_LED_FLUSH_LIMIT 26
#define RGB_MATRIX_SLEEP // turn off effects when suspended
#define RGB_MATRIX_DEFAULT_HUE 170
#define RGB_MATRIX_DEFAULT_SAT 255
#define RGB_MATRIX_DEFAULT_VAL 48
#define RGB_MATRIX_DEFAULT_SPD 32
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_CYCLE_LEFT_RIGHT
#endif

#define NO_SUSPEND_POWER_DOWN

/*
 * Keyboard Matrix Assignments
 *
 * Change this to how you wired your keyboard
 * COLS: AVR pins used for columns, left to right
 * ROWS: AVR pins used for rows, top to bottom
 * DIODE_DIRECTION: COL2ROW = COL = Anode (+), ROW = Cathode (-, marked on
 * diode) ROW2COL = ROW = Anode (+), COL = Cathode (-, marked on diode)
 *
 */
#define MATRIX_ROW_PINS {A7, A6, A5, A4, A3}
#define MATRIX_COL_PINS                                                        \
  {C15, C0, C1, C2, C3, A0, A1, B12, C6, C7, C8, C9, C10, C11, C12}

#define DIODE_DIRECTION COL2ROW

/* Dip Switch */
#define DIP_SWITCH_PINS {B3}
// #define DIP_SWITCH_PULLUP

/* RGB Matrix config */
#define RGBLED_NUM 67
#define RGB_MATRIX_LED_COUNT RGBLED_NUM
#define RGB_EN_PIN B5
#define WS2812_DI_PIN B15

// PWM RGB Underglow Defines
#define WS2812_SPI SPID2
#define WS2812_SPI_MOSI_PAL_MODE 0
#define WS2812_SPI_SCK_PAL_MODE 0
#define WS2812_SPI_SCK_PIN B13
#define WS2812_BYTE_ORDER WS2812_BYTE_ORDER_GRB
#define WS2812_TRST_US 200

// I2C config
#define I2C_DRIVER I2CD1
#define I2C1_SCL_PIN B6
#define I2C1_SDA_PIN B7
#define I2C1_SCL_PAL_MODE 1
#define I2C1_SDA_PAL_MODE 1
#define I2C1_TIMINGR_PRESC 0x00U
#define I2C1_TIMINGR_SCLDEL 0x03U
#define I2C1_TIMINGR_SDADEL 0x01U
#define I2C1_TIMINGR_SCLH 0x03U
#define I2C1_TIMINGR_SCLL 0x09U

// #define I2C_IRQ_PIN C14

/* Debug Output */
// #define DEBUG_ENABLE

#if (!defined(CONSOLE_ENABLE) && defined(DEBUG_ENABLE))
#define DEBUG_DRIVER SD3
#undef NO_PRINT
#undef NO_DEBUG
#endif
#define DEBUG_TX_PIN B10
#define DEBUG_TX_PAL_MODE 4
#define DEBUG_RX_PIN B11
#define DEBUG_RX_PAL_MODE 4
