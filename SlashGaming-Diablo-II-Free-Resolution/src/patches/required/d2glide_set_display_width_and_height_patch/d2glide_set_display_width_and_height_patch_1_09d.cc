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

#include "d2glide_set_display_width_and_height_patch_1_09d.hpp"

#include <array>

#include "../../../asm_x86_macro.h"
#include "d2glide_set_display_width_and_height.hpp"

namespace sgd2fr::patches::d2glide {
namespace {

__declspec(naked) void __cdecl InterceptionFunc_01() {
  ASM_X86(push ebp);
  ASM_X86(mov ebp, esp);

  ASM_X86(push edx);

  // Create space for 3 variables, (width, height, glide_res_id)
  ASM_X86(sub esp, 12);

  ASM_X86(lea ebx, [esp]);
  ASM_X86(lea eax, [esp + 4]);
  ASM_X86(lea ecx, [esp + 8]);

  ASM_X86(push ecx);
  ASM_X86(push eax);
  ASM_X86(push ebx);
  ASM_X86(push esi);
  ASM_X86(call ASM_X86_FUNC(Sgd2fr_D2Glide_SetDisplayWidthAndHeight));
  ASM_X86(add esp, 16);

  // Load the values to set up the proper state.
  ASM_X86(mov eax, [ebx]);
  ASM_X86(mov ecx, [ebx + 4]);
  ASM_X86(mov ebx, [ebx + 8]);

  ASM_X86(pop edx);

  ASM_X86(leave);
  ASM_X86(ret);
}

/**
 * nop 
 * nop 
 * nop 
 * nop 
 * nop 
 * cmp eax, 1
 * jne D2Glide.dll+1BD1
 */
constexpr std::array<std::uint8_t, 10> kPatchBuffer02 = {
    0x90, 0x90, 0x90, 0x90, 0x90, 0x83, 0xF8, 0x01, 0x75, 0x11
};

} // namespace

SetDisplayWidthAndHeightPatch_1_09D::SetDisplayWidthAndHeightPatch_1_09D()
  : patches_(MakePatches()) {
}

void SetDisplayWidthAndHeightPatch_1_09D::Apply() {
  for (auto& patch : this->patches_) {
    patch.Apply();
  }
}

void SetDisplayWidthAndHeightPatch_1_09D::Remove() {
  for (auto& patch : this->patches_) {
    patch.Apply();
  }
}

std::vector<mapi::GamePatch>
SetDisplayWidthAndHeightPatch_1_09D::MakePatches() {
  std::vector<mapi::GamePatch> patches;

  PatchAddressAndSize patch_address_and_size_01 =
      GetPatchAddressAndSize01();
  patches.push_back(
      mapi::GamePatch::MakeGameBranchPatch(
          patch_address_and_size_01.first,
          mapi::BranchType::kCall,
          &InterceptionFunc_01,
          patch_address_and_size_01.second
      )
  );

  PatchAddressAndSize patch_address_and_size_02 =
      GetPatchAddressAndSize02();
  patches.push_back(
      mapi::GamePatch::MakeGameBufferPatch(
          patch_address_and_size_02.first,
          kPatchBuffer02.data(),
          patch_address_and_size_02.second
      )
  );

  return patches;
}

SetDisplayWidthAndHeightPatch_1_09D::PatchAddressAndSize
SetDisplayWidthAndHeightPatch_1_09D::GetPatchAddressAndSize01() {
  ::d2::GameVersion running_game_version = ::d2::game_version::GetRunning();

  switch (running_game_version) {
    case ::d2::GameVersion::k1_09D: {
      return PatchAddressAndSize(
          ::mapi::GameAddress::FromOffset(
              ::d2::DefaultLibrary::kD2Glide,
              0x1B8B
          ),
          0x1B9F - 0x1B8B
      );
    }
  }
}

SetDisplayWidthAndHeightPatch_1_09D::PatchAddressAndSize
SetDisplayWidthAndHeightPatch_1_09D::GetPatchAddressAndSize02() {
  ::d2::GameVersion running_game_version = ::d2::game_version::GetRunning();

  switch (running_game_version) {
    case ::d2::GameVersion::k1_09D: {
      return PatchAddressAndSize(
          ::mapi::GameAddress::FromOffset(
              ::d2::DefaultLibrary::kD2Glide,
              0x1BB6
          ),
          kPatchBuffer02.size()
      );
    }
  }
}

} // namespace sgd2fr::patches::d2glide