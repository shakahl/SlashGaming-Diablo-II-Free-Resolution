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

#include "glide3x_gr_sst_win_open_patch.hpp"

#include <stddef.h>

#include <sgd2mapi.hpp>
#include "../../../sgd2mapi_extension.h"

namespace sgd2fr {
namespace glide3x {

GrSstWinOpenPatch::GrSstWinOpenPatch()
    : AbstractMultiversionPatch(IsApplicable(), InitPatch()) {
}

bool GrSstWinOpenPatch::IsApplicable() {
  ::d2::VideoMode video_mode = ::d2::helper::DetermineVideoMode();
  if (video_mode != ::d2::VideoMode::kGlide) {
    return false;
  }

  // The D2DX API extensions are used instead.
  D2_Glide3xLibraryVersion running_glide3x_library_version =
      D2_Glide3xLibraryVersion_GetRunning();
  return (running_glide3x_library_version
      != D2_Glide3xLibraryVersion_kD2dx);
}

AbstractVersionPatch*
GrSstWinOpenPatch::InitPatch() {
  if (!IsApplicable()) {
    return NULL;
  }

  switch (D2_Glide3xLibraryVersion_GetRunning()) {
    case D2_Glide3xLibraryVersion_kSven1_4_4_21:
    case D2_Glide3xLibraryVersion_kSven1_4_6_1: {
      return new GrSstWinOpenPatch_Sven_1_4_4_21();
    }

    case D2_Glide3xLibraryVersion_kSven1_4_8_3: {
      return new GrSstWinOpenPatch_Sven_1_4_8_3();
    }

    case D2_Glide3xLibraryVersion_kNGlide3_10_0_658: {
      return new GrSstWinOpenPatch_NGlide_3_10_0_658();
    }

    case D2_Glide3xLibraryVersion_kD2dx: {
      return NULL;
    }
  }
}

} // namespace glide3x
} // namespace sgd2fr
