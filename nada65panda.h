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

#include "quantum.h"

#define XXX KC_NO

// Defining all the custom keycodes.
enum custom_keycodes {
#ifdef VIA_ENABLE
  DEV_BT1 = QK_KB_0,
#else
  DEV_BT1 = SAFE_RANGE,
#endif
  DEV_BT2,
  DEV_BT3,
  DEV_RF24,
  DEV_CLR,
  W_LOCK,
  RGB_RST,
  RGB_COLOR,
  S_MODE, // SIDE_LED
  S_VALUE,
  S_COLOR,
  S_SPEED,
  NEW_SAFE_RANGE
};

#undef SAFE_RANGE
#define SAFE_RANGE NEW_SAFE_RANGE
