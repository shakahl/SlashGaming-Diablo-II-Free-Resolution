/**
 * SlashGaming Diablo II Free Resolution
 * Copyright (C) 2019-2021  Mir Drualga
 *
 * This file is part of SlashGaming Diablo II Free Resolution.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Additional permissions under GNU Affero General Public License version 3
 *  section 7
 *
 *  If you modify this Program, or any covered work, by linking or combining
 *  it with Diablo II (or a modified version of that game and its
 *  libraries), containing parts covered by the terms of Blizzard End User
 *  License Agreement, the licensors of this Program grant you additional
 *  permission to convey the resulting work. This additional permission is
 *  also extended to any combination of expansions, mods, and remasters of
 *  the game.
 *
 *  If you modify this Program, or any covered work, by linking or combining
 *  it with any Graphics Device Interface (GDI), DirectDraw, Direct3D,
 *  Glide, OpenGL, or Rave wrapper (or modified versions of those
 *  libraries), containing parts not covered by a compatible license, the
 *  licensors of this Program grant you additional permission to convey the
 *  resulting work.
 *
 *  If you modify this Program, or any covered work, by linking or combining
 *  it with any library (or a modified version of that library) that links
 *  to Diablo II (or a modified version of that game and its libraries),
 *  containing parts not covered by a compatible license, the licensors of
 *  this Program grant you additional permission to convey the resulting
 *  work.
 */

#ifndef SGD2FR_COMPILE_TIME_SWITCH_H_
#define SGD2FR_COMPILE_TIME_SWITCH_H_

/**
 * Strictly a place where compile-time switches can be easily changed
 * to alter software behavior.
 */

/**
 * This is a temporary option to aid in the port to Visual C++ 6.0.
 * This sets the cel file path version to use.
 *
 * 2 = Paths prior to 3.0.2.1 and below
 * 3 = Paths for 3.1.0.0 and beyond
 */
#define COMPILE_TIME_CEL_FILE_PATH_VERSION 2

/**
 * Specifies the config format to use when creating a new config,
 * using the following integer values to represent a format type:
 * 0. INI (Windows)
 * 1. JSON (frozen)
 */
#define COMPILE_TIME_CREATE_CONFIG_FORMAT 1

/**
 * If 1, the inventory arrangement sources from 800x600 entries in
 * calculations. Otherwise, sources from 640x480 entries.
 */
#define COMPILE_TIME_IS_SOURCE_INVENTORY_ARRANGE_800 1

#endif /* SGD2FR_COMPILE_TIME_SWITCH_H_ */
