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

#include "d2client_draw_screen_background.hpp"

#include <stddef.h>
#include <windows.h>

#include <mdc/error/exit_on_error.hpp>
#include <mdc/wchar_t/filew.h>
#include <sgd2mapi.hpp>
#include "../../../config.hpp"
#include "../../../helper/cel_file_collection.hpp"
#include "../../../helper/game_resolution.hpp"
#include "../../../sgd2mapi_extension/sgd2mapi_extension.hpp"

namespace sgd2fr {
namespace {

constexpr std::string_view kOriginalScreenBorderFrameImagePath =
    "data\\global\\UI\\Panel\\800BorderFrame";

void DrawLeftScreenBackground() {
  ::d2::CelFile_Api& screen_background = GetCelFile(config::GetScreenBackgroundImagePath());

  std::tuple width_and_height = GetIngameResolutionFromId(d2::d2gfx::GetResolutionMode());

  // Draw a black rectangle to stop transparent DC6 cheaters.
  ::d2::d2gfx::DrawRectangle(
      0,
      0,
      std::get<0>(width_and_height) / 2,
      std::get<1>(width_and_height),
      0,
      ::d2::DrawEffect::kNone
  );


  // Draw the background.
  const size_t half_num_frames = screen_background.GetNumFrames() / 2;

  int height_covered = 0;
  int width_covered = 0;
  size_t column = 0;
  size_t row = 0;

  while (height_covered < std::get<1>(width_and_height)) {
    const size_t frame = ((row % 2) * half_num_frames)
        + (column % half_num_frames);

    ::d2::Cel_Wrapper cel = screen_background.GetCel(0, frame);

    screen_background.DrawFrame(
        (std::get<0>(width_and_height) / 2) - width_covered - cel.GetWidth(),
        height_covered + cel.GetHeight(),
        0,
        frame
    );

    width_covered += cel.GetWidth();
    if (width_covered < std::get<0>(width_and_height)) {
      column += 1;
    } else {
      width_covered = 0;
      column = 0;

      height_covered += cel.GetHeight();
      row += 1;
    }
  }
}

void DrawRightScreenBackground() {
  ::d2::CelFile_Api& screen_background = GetCelFile(config::GetScreenBackgroundImagePath());

  std::tuple width_and_height = GetIngameResolutionFromId(d2::d2gfx::GetResolutionMode());

  // Draw a black rectangle to stop transparent DC6 cheaters.
  ::d2::d2gfx::DrawRectangle(
      std::get<0>(width_and_height) / 2,
      0,
      std::get<0>(width_and_height),
      std::get<1>(width_and_height),
      0,
      ::d2::DrawEffect::kNone
  );

  // Draw the background.
  const size_t half_num_frames = screen_background.GetNumFrames() / 2;

  int height_covered = 0;
  int width_covered = 0;
  size_t column = 0;
  size_t row = 0;

  while (height_covered < std::get<1>(width_and_height)) {
    const size_t frame = ((row % 2) * half_num_frames)
        + (column % half_num_frames);

    ::d2::Cel_Wrapper cel = screen_background.GetCel(0, frame);

    screen_background.DrawFrame(
        width_covered,
        height_covered + cel.GetHeight(),
        0,
        frame
    );

    width_covered += cel.GetWidth();
    if (width_covered < std::get<0>(width_and_height)) {
      column += 1;
    } else {
      width_covered = 0;
      column = 0;

      height_covered += cel.GetHeight();
      row += 1;
    }
  }
}

/**
 * Drawing the original screen border frame requires significantly
 * different code due to its abnormal structure.
 */
void DrawOriginalLeftScreenBorderFrame() {
  enum {
    kNumFramesOnLeft = 5,
    kNumFramesOnRight = 4,
  };

  ::d2::CelFile_Api& screen_border_frame = GetCelFile(
      kOriginalScreenBorderFrameImagePath
  );

  std::tuple width_and_height = GetIngameResolutionFromId(
      d2::d2gfx::GetResolutionMode()
  );

  const int left = (std::get<0>(width_and_height) - 640 - (800 - 640)) / 2;
  const int top = ((std::get<1>(width_and_height) - 480 - (600 - 480)) / 2) - 3;

  ::d2::Cel_Wrapper cels[kNumFramesOnLeft] = {
      screen_border_frame.GetCel(0, 0),
      screen_border_frame.GetCel(0, 1),
      screen_border_frame.GetCel(0, 2),
      screen_border_frame.GetCel(0, 3),
      screen_border_frame.GetCel(0, 4),
  };

  screen_border_frame.DrawFrame(
      left,
      top + cels[0].GetHeight(),
      0,
      0
  );

  screen_border_frame.DrawFrame(
      left + cels[0].GetWidth(),
      top + cels[1].GetHeight(),
      0,
      1
  );

  screen_border_frame.DrawFrame(
      left,
      top + cels[0].GetHeight() + cels[2].GetHeight(),
      0,
      2
  );

  screen_border_frame.DrawFrame(
      left,
      top + cels[0].GetHeight() + cels[2].GetHeight() + cels[3].GetHeight(),
      0,
      3
  );

  screen_border_frame.DrawFrame(
      left + cels[3].GetWidth(),
      top + cels[0].GetHeight() + cels[2].GetHeight() + cels[4].GetHeight(),
      0,
      4
  );
}

void DrawOriginalRightScreenBorderFrame() {
  enum {
    kNumFramesOnLeft = 5,
    kNumFramesOnRight = 5,
  };

  ::d2::CelFile_Api& screen_border_frame = GetCelFile(
      kOriginalScreenBorderFrameImagePath
  );

  std::tuple width_and_height = GetIngameResolutionFromId(d2::d2gfx::GetResolutionMode());

  const int right = (std::get<0>(width_and_height) + 640 + (800 - 640)) / 2;
  const int top = ((std::get<1>(width_and_height) - 480 - (600 - 480)) / 2) - 3;

  ::d2::Cel_Wrapper cels[kNumFramesOnRight] = {
      screen_border_frame.GetCel(0, kNumFramesOnLeft),
      screen_border_frame.GetCel(0, kNumFramesOnLeft + 1),
      screen_border_frame.GetCel(0, kNumFramesOnLeft + 2),
      screen_border_frame.GetCel(0, kNumFramesOnLeft + 3),
      screen_border_frame.GetCel(0, kNumFramesOnLeft + 4),
  };

  screen_border_frame.DrawFrame(
      right - cels[6 - kNumFramesOnLeft].GetWidth(),
      top + cels[6 - kNumFramesOnLeft].GetHeight(),
      0,
      6
  );

  screen_border_frame.DrawFrame(
      right - cels[6 - kNumFramesOnLeft].GetWidth()
          - cels[5 - kNumFramesOnLeft].GetWidth(),
      top + cels[5 - kNumFramesOnLeft].GetHeight(),
      0,
      5
  );

  screen_border_frame.DrawFrame(
      right - cels[6 - kNumFramesOnLeft].GetWidth(),
      top + cels[6 - kNumFramesOnLeft].GetHeight()
          + cels[7 - kNumFramesOnLeft].GetHeight(),
      0,
      7
  );

  screen_border_frame.DrawFrame(
      right - cels[8 - kNumFramesOnLeft].GetWidth(),
      top + cels[6 - kNumFramesOnLeft].GetHeight()
          + cels[7 - kNumFramesOnLeft].GetHeight()
          + cels[8 - kNumFramesOnLeft].GetHeight(),
      0,
      8
  );

  screen_border_frame.DrawFrame(
      right - cels[8 - kNumFramesOnLeft].GetWidth(),
      top + cels[6 - kNumFramesOnLeft].GetHeight()
          + cels[7 - kNumFramesOnLeft].GetHeight()
          + cels[9 - kNumFramesOnLeft].GetHeight(),
      0,
      9
  );
}

void DrawCustomLeftScreenBorderFrame() {
  ::d2::CelFile_Api& screen_left_border = GetCelFile(config::GetCustomLeftScreenBorderLeftImagePath());
  ::d2::CelFile_Api& screen_top_border = GetCelFile(config::GetCustomLeftScreenBorderTopImagePath());
  ::d2::CelFile_Api& screen_top_right_border = GetCelFile(config::GetCustomLeftScreenBorderTopRightImagePath());
  ::d2::CelFile_Api& screen_bottom_border = GetCelFile(config::GetCustomLeftScreenBorderBottomImagePath());
  ::d2::CelFile_Api& screen_bottom_right_border = GetCelFile(config::GetCustomLeftScreenBorderBottomRightImagePath());

  const std::tuple width_and_height = GetIngameResolutionFromId(d2::d2gfx::GetResolutionMode());

  // Determine border starting positions.
  const int screen_left = (std::get<0>(width_and_height) - 640) / 2;
  const int screen_top = (std::get<1>(width_and_height) - 480) / 2;

  int screen_left_border_height = 0;

  for (size_t frame_index = 0;
      frame_index < screen_left_border.GetNumFrames();
      frame_index += 1
  ) {
    ::d2::Cel_View cel = screen_left_border.GetCel(0, frame_index);
    screen_left_border_height += cel.GetHeight();
  }

  const int border_top = screen_top + (((256 + 176) - screen_left_border_height) / 2);
  const int border_bottom = screen_top + (((256 + 176) + screen_left_border_height) / 2);

  // Draw the left screen's top right border.
  int screen_top_right_border_width = 0;

  for (size_t frame_index = screen_top_right_border.GetNumFrames() - 1;
      (frame_index + 1) > 0;
      frame_index -= 1
  ) {
    ::d2::Cel_Wrapper cel = screen_top_right_border.GetCel(
        0,
        frame_index
    );

    screen_top_right_border.DrawFrame(
        (std::get<0>(width_and_height) / 2)
            - screen_top_right_border_width
            - cel.GetWidth(),
        border_top + cel.GetHeight(),
        0,
        frame_index
    );

    screen_top_right_border_width += cel.GetWidth();
  }

  // Draw the left screen's top border.
  int screen_top_border_width = 0;

  for (size_t frame_index = screen_top_border.GetNumFrames() - 1;
      (frame_index + 1) > 0;
      frame_index -= 1
  ) {
    ::d2::Cel_Wrapper cel = screen_top_border.GetCel(0, frame_index);

    screen_top_border.DrawFrame(
        (std::get<0>(width_and_height) / 2)
            - screen_top_right_border_width
            - cel.GetWidth(),
        border_top + cel.GetHeight(),
        0,
        frame_index
    );

    screen_top_border_width += cel.GetWidth();
  }

  // Draw the left screen's bottom right border.
  int screen_bottom_right_border_width = 0;

  for (size_t frame_index = screen_bottom_right_border.GetNumFrames() - 1;
      (frame_index + 1) > 0;
      frame_index -= 1
  ) {
    ::d2::Cel_Wrapper cel = screen_bottom_right_border.GetCel(0, frame_index);

    screen_bottom_right_border.DrawFrame(
        (std::get<0>(width_and_height) / 2)
            - screen_bottom_right_border_width
            - cel.GetWidth(),
        border_bottom,
        0,
        frame_index
    );

    screen_bottom_right_border_width += cel.GetWidth();
  }

  // Draw the left screen's bottom border.
  int screen_bottom_border_width = 0;

  for (size_t frame_index = screen_bottom_border.GetNumFrames() - 1;
      (frame_index + 1) > 0;
      frame_index -= 1) {
    ::d2::Cel_Wrapper cel = screen_bottom_border.GetCel(0, frame_index);

    screen_bottom_border.DrawFrame(
        (std::get<0>(width_and_height) / 2)
            - screen_bottom_right_border_width
            - cel.GetWidth(),
        border_bottom,
        0,
        frame_index
    );

    screen_bottom_border_width += cel.GetWidth();
  }

  // Draw the left screen's left border.
  // TODO (Mir Drualga): Definition disabled to suppress error.
  /*int*/ screen_left_border_height = 0;

  for (size_t frame_index = 0;
      frame_index < screen_left_border.GetNumFrames();
      frame_index += 1
  ) {
    ::d2::Cel_Wrapper cel = screen_left_border.GetCel(0, frame_index);

    screen_left_border.DrawFrame(
        ((std::get<0>(width_and_height) - 640) / 2) - cel.GetWidth(),
        border_top + screen_left_border_height + cel.GetHeight(),
        0,
        frame_index
    );

    screen_left_border_height += cel.GetHeight();
  }
}

void DrawCustomRightScreenBorderFrame() {
  ::d2::CelFile_Api& screen_right_border = GetCelFile(config::GetCustomRightScreenBorderRightImagePath());
  ::d2::CelFile_Api& screen_top_border = GetCelFile(config::GetCustomRightScreenBorderTopImagePath());
  ::d2::CelFile_Api& screen_top_left_border = GetCelFile(config::GetCustomRightScreenBorderTopLeftImagePath());
  ::d2::CelFile_Api& screen_bottom_border = GetCelFile(config::GetCustomRightScreenBorderBottomImagePath());
  ::d2::CelFile_Api& screen_bottom_left_border = GetCelFile(config::GetCustomRightScreenBorderBottomLeftImagePath());

  const std::tuple width_and_height = GetIngameResolutionFromId(d2::d2gfx::GetResolutionMode());

  // Determine border starting positions.
  const int screen_right = (std::get<0>(width_and_height) + 640) / 2;
  const int screen_top = (std::get<1>(width_and_height) - 480) / 2;

  int screen_right_border_height = 0;

  for (unsigned int frame = 0; frame < screen_right_border.GetNumFrames(); frame += 1) {
    ::d2::Cel_View cel_view = screen_right_border.GetCel(0, frame);
    screen_right_border_height += cel_view.GetHeight();
  }

  const int border_top = screen_top + (((256 + 176) - screen_right_border_height) / 2);
  const int border_bottom = screen_top + (((256 + 176) + screen_right_border_height) / 2);

  // Draw the right screen's top left border.
  int screen_top_left_border_width = 0;

  for (size_t frame_index = 0;
      frame_index < screen_top_left_border.GetNumFrames();
      frame_index += 1
  ) {
    ::d2::Cel_Wrapper cel = screen_top_left_border.GetCel(0, frame_index);

    screen_top_left_border.DrawFrame(
        (std::get<0>(width_and_height) / 2) + screen_top_left_border_width,
        border_top + cel.GetHeight(),
        0,
        frame_index
    );

    screen_top_left_border_width += cel.GetWidth();
  }

  // Draw the right screen's top border.
  int screen_top_border_width = 0;

  for (size_t frame_index = 0;
      frame_index < screen_top_border.GetNumFrames();
      frame_index += 1
  ) {
    ::d2::Cel_Wrapper cel = screen_top_border.GetCel(0, frame_index);

    screen_top_border.DrawFrame(
        (std::get<0>(width_and_height) / 2)
            + screen_top_left_border_width
            + screen_top_border_width,
        border_top + cel.GetHeight(),
        0,
        frame_index
    );

    screen_top_border_width += cel.GetWidth();
  }

  // Draw the right screen's bottom left border.
  int screen_bottom_left_border_width = 0;

  for (size_t frame_index = 0;
      frame_index < screen_bottom_left_border.GetNumFrames();
      frame_index += 1
  ) {
    ::d2::Cel_Wrapper cel = screen_bottom_left_border.GetCel(0, frame_index);

    screen_bottom_left_border.DrawFrame(
        (std::get<0>(width_and_height) / 2) + screen_bottom_left_border_width,
        border_bottom,
        0,
        frame_index
    );

    screen_bottom_left_border_width += cel.GetWidth();
  }

  // Draw the right screen's bottom border.
  int screen_bottom_border_width = 0;

  for (size_t frame_index = 0;
      frame_index < screen_bottom_border.GetNumFrames();
      frame_index += 1
  ) {
    ::d2::Cel_Wrapper cel = screen_bottom_border.GetCel(0, frame_index);

    screen_bottom_border.DrawFrame(
        (std::get<0>(width_and_height) / 2)
            + screen_bottom_left_border_width
            + screen_bottom_border_width,
        border_bottom,
        0,
        frame_index
    );

    screen_bottom_border_width += cel.GetWidth();
  }

  // Draw the right screen's right border.
  // TODO (Mir Drualga): Definition disabled to suppress error.
  /*int*/ screen_right_border_height = 0;

  for (size_t frame_index = 0;
      frame_index < screen_right_border.GetNumFrames();
      frame_index += 1
  ) {
    ::d2::Cel_Wrapper cel = screen_right_border.GetCel(0, frame_index);

    screen_right_border.DrawFrame(
        screen_right,
        border_top + screen_right_border_height + cel.GetHeight(),
        0,
        frame_index
    );

    screen_right_border_height += cel.GetHeight();
  }
}

static void DrawLeftScreenBorderFrame() {
  if (!config::IsScreenBorderFrameEnabled()) {
    return;
  }

  if (config::IsUseOriginalScreenBorderFrame()) {
    DrawOriginalLeftScreenBorderFrame();
  } else {
    DrawCustomLeftScreenBorderFrame();
  }
}

static void DrawRightScreenBorderFrame() {
  if (!config::IsScreenBorderFrameEnabled()) {
    return;
  }

  if (config::IsUseOriginalScreenBorderFrame()) {
    DrawOriginalRightScreenBorderFrame();
  } else {
    DrawCustomRightScreenBorderFrame();
  }
}

static void DrawLeftScreenBackgroundHorizontalRibbons() {
  ::d2::CelFile_Api& screen_border_horizontal_ribbon = GetCelFile(
      config::GetScreenBorderHorizontalRibbonImagePath()
  );

  ::d2::CelFile_Api& screen_left_border = GetCelFile(
      config::GetCustomLeftScreenBorderLeftImagePath()
  );

  // Determine border starting positions.
  const std::tuple width_and_height = GetIngameResolutionFromId(
      d2::d2gfx::GetResolutionMode()
  );

  const int screen_left = (std::get<0>(width_and_height) - 640) / 2;
  const int screen_right = std::get<0>(width_and_height) / 2;
  const int screen_top = (std::get<1>(width_and_height) - 480) / 2;

  int screen_left_border_height = 0;

  for (size_t frame_index = 0;
      frame_index < screen_left_border.GetNumFrames();
      frame_index += 1
  ) {
    ::d2::Cel_View cel = screen_left_border.GetCel(0, frame_index);
    screen_left_border_height += cel.GetHeight();
  }

  const int border_top = screen_top + (((256 + 176) - screen_left_border_height) / 2);
  const int border_left = screen_left - screen_left_border.GetCel(0, 0).GetWidth();
  const int border_bottom = screen_top + (((256 + 176) + screen_left_border_height) / 2);

  // Draw horizontal bars.
  int width_covered = 0;

  for (size_t frame_index = 0;
      width_covered < border_left;
      frame_index += 1
  ) {
    frame_index %= screen_border_horizontal_ribbon.GetNumFrames();

    ::d2::Cel_Wrapper cel = screen_border_horizontal_ribbon.GetCel(
        0,
        frame_index
    );

    screen_border_horizontal_ribbon.DrawFrame(
        border_left - width_covered - cel.GetWidth(),
        border_top + cel.GetHeight(),
        0,
        frame_index
    );

    screen_border_horizontal_ribbon.DrawFrame(
        border_left - width_covered - cel.GetWidth(),
        border_bottom,
        0,
        frame_index
    );

    width_covered += cel.GetWidth();
  }
}

static void DrawLeftScreenBackgroundVerticalRibbons() {
  ::d2::CelFile_Api& screen_border_vertical_ribbon = GetCelFile(
      config::GetScreenBorderVerticalRibbonImagePath()
  );

  ::d2::CelFile_Api& screen_left_border = GetCelFile(
      config::GetCustomLeftScreenBorderLeftImagePath()
  );

  // Determine border starting positions.
  const std::tuple width_and_height = GetIngameResolutionFromId(
      d2::d2gfx::GetResolutionMode()
  );

  const int screen_left = (std::get<0>(width_and_height) - 640) / 2;
  const int screen_right = std::get<0>(width_and_height) / 2;
  const int screen_top = (std::get<1>(width_and_height) - 480) / 2;

  int screen_left_border_height = 0;

  for (size_t frame_index = 0;
      frame_index < screen_left_border.GetNumFrames();
      frame_index += 1
  ) {
    ::d2::Cel_View cel = screen_left_border.GetCel(0, frame_index);
    screen_left_border_height += cel.GetHeight();
  }

  const int border_top = screen_top + (((256 + 176) - screen_left_border_height) / 2);
  const int border_left = screen_left - screen_left_border.GetCel(0, 0).GetWidth();
  const int border_bottom = screen_top + (((256 + 176) + screen_left_border_height) / 2);

  // Draw top vertical bars.
  int top_height_covered = 0;

  for (size_t frame_index = 0;
      top_height_covered < border_top;
      frame_index += 1
  ) {
    frame_index %= screen_border_vertical_ribbon.GetNumFrames();

    ::d2::Cel_Wrapper cel = screen_border_vertical_ribbon.GetCel(
        0,
        frame_index
    );

    screen_border_vertical_ribbon.DrawFrame(
        border_left,
        border_top - top_height_covered,
        0,
        frame_index
    );

    screen_border_vertical_ribbon.DrawFrame(
        screen_right - cel.GetWidth(),
        border_top - top_height_covered,
        0,
        frame_index
    );

    top_height_covered += cel.GetHeight();
  }

  // Draw bottom vertical bars.
  int bottom_height_covered = 0;

  for (size_t frame_index = 0;
      bottom_height_covered < border_top;
      frame_index += 1
  ) {
    frame_index %= screen_border_vertical_ribbon.GetNumFrames();

    ::d2::Cel_Wrapper cel = screen_border_vertical_ribbon.GetCel(
        0,
        frame_index
    );

    screen_border_vertical_ribbon.DrawFrame(
        border_left,
        border_bottom + bottom_height_covered + cel.GetHeight(),
        0,
        frame_index
    );

    screen_border_vertical_ribbon.DrawFrame(
        screen_right - cel.GetWidth(),
        border_bottom + bottom_height_covered + cel.GetHeight(),
        0,
        frame_index
    );

    bottom_height_covered += cel.GetWidth();
  }
}

void DrawLeftScreenBackgroundRibbon() {
  DrawLeftScreenBackgroundHorizontalRibbons();
  DrawLeftScreenBackgroundVerticalRibbons();
}

static void DrawRightScreenBackgroundHorizontalRibbon() {
  ::d2::CelFile_Api& screen_border_horizontal_ribbon = GetCelFile(
      config::GetScreenBorderHorizontalRibbonImagePath()
  );

  ::d2::CelFile_Api& screen_right_border = GetCelFile(
      config::GetCustomRightScreenBorderRightImagePath()
  );

  // Determine border starting positions.
  const ::std::tuple width_and_height = GetIngameResolutionFromId(
      d2::d2gfx::GetResolutionMode()
  );

  const int screen_left = std::get<0>(width_and_height) / 2;
  const int screen_right = (std::get<0>(width_and_height) + 640) / 2;
  const int screen_top = (std::get<1>(width_and_height) - 480) / 2;

  int screen_right_border_height = 0;

  for (size_t frame_index = 0;
      frame_index < screen_right_border.GetNumFrames();
      frame_index += 1
  ) {
    ::d2::Cel_Wrapper cel = screen_right_border.GetCel(0, frame_index);
    screen_right_border_height += cel.GetHeight();
  }

  const int border_top = screen_top + (((256 + 176) - screen_right_border_height) / 2);
  const int border_right = screen_right + screen_right_border.GetCel(0, 0).GetWidth();
  const int border_bottom = border_top + screen_right_border_height;

  // Draw horizontal bars.
  size_t frame_index = 0;
  int width_covered = 0;

  for (size_t frame_index = 0;
      width_covered < (std::get<0>(width_and_height) - border_right);
      frame_index += 1
  ) {
    frame_index %= screen_border_horizontal_ribbon.GetNumFrames();

    ::d2::Cel_Wrapper cel = screen_border_horizontal_ribbon.GetCel(
        0,
        frame_index
    );

    screen_border_horizontal_ribbon.DrawFrame(
        border_right + width_covered,
        border_top + cel.GetHeight(),
        0,
        frame_index
    );

    screen_border_horizontal_ribbon.DrawFrame(
        border_right + width_covered,
        border_bottom,
        0,
        frame_index
    );

    width_covered += cel.GetWidth();
  }
}

static void DrawRightScreenBackgroundVerticalRibbons() {
  ::d2::CelFile_Api& screen_border_vertical_ribbon = GetCelFile(
      config::GetScreenBorderVerticalRibbonImagePath()
  );

  ::d2::CelFile_Api& screen_right_border = GetCelFile(
      config::GetCustomRightScreenBorderRightImagePath()
  );

  // Determine border starting positions.
  const std::tuple width_and_height = GetIngameResolutionFromId(
      d2::d2gfx::GetResolutionMode()
  );

  const int screen_left = std::get<0>(width_and_height) / 2;
  const int screen_right = (std::get<0>(width_and_height) + 640) / 2;
  const int screen_top = (std::get<1>(width_and_height) - 480) / 2;

  int screen_right_border_height = 0;

  for (size_t frame_index = 0;
      frame_index < screen_right_border.GetNumFrames();
      frame_index += 1
  ) {
    ::d2::Cel_View cel = screen_right_border.GetCel(0, frame_index);
    screen_right_border_height += cel.GetHeight();
  }

  const int border_top = screen_top + (((256 + 176) - screen_right_border_height) / 2);
  const int border_right = screen_right + screen_right_border.GetCel(0, 0).GetWidth();
  const int border_bottom = border_top + screen_right_border_height;

  // Draw top vertical bars.
  int top_height_covered = 0;

  for (size_t frame_index = 0;
      top_height_covered < border_top;
      frame_index += 1
  ) {
    frame_index %= screen_border_vertical_ribbon.GetNumFrames();

    ::d2::Cel_Wrapper cel = screen_border_vertical_ribbon.GetCel(
        0,
        frame_index
    );

    screen_border_vertical_ribbon.DrawFrame(
        screen_left,
        border_top - top_height_covered,
        0,
        frame_index
    );

    screen_border_vertical_ribbon.DrawFrame(
        border_right - cel.GetWidth(),
        border_top - top_height_covered,
        0,
        frame_index
    );

    top_height_covered += cel.GetHeight();
  }

  // Draw bottom vertical bars.
  int bottom_height_covered = 0;

  for (size_t frame_index = 0;
      bottom_height_covered < border_top;
      frame_index += 1
  ) {
    frame_index %= screen_border_vertical_ribbon.GetNumDirections();

    ::d2::Cel_Wrapper cel = screen_border_vertical_ribbon.GetCel(
        0,
        frame_index
    );

    screen_border_vertical_ribbon.DrawFrame(
        screen_left,
        border_bottom + bottom_height_covered + cel.GetHeight(),
        0,
        frame_index
    );

    screen_border_vertical_ribbon.DrawFrame(
        border_right - cel.GetWidth(),
        border_bottom + bottom_height_covered + cel.GetHeight(),
        0,
        frame_index
    );

    bottom_height_covered += cel.GetWidth();
  }
}

void DrawRightScreenBackgroundRibbon() {
  DrawRightScreenBackgroundHorizontalRibbon();
  DrawRightScreenBackgroundVerticalRibbons();
}

} // namespace

void Sgd2fr_D2Client_DrawScreenBackground() {
  ::d2::ScreenOpenMode screen_open_mode =
      ::d2::d2client::GetScreenOpenMode();

  switch (screen_open_mode) {
    case ::d2::ScreenOpenMode::kNone: {
      break;
    }

    case ::d2::ScreenOpenMode::kRight: {
      DrawRightScreenBackground();
      DrawRightScreenBorderFrame();
      DrawRightScreenBackgroundRibbon();

      break;
    }

    case ::d2::ScreenOpenMode::kLeft: {
      DrawLeftScreenBackground();
      DrawLeftScreenBorderFrame();
      DrawLeftScreenBackgroundRibbon();

      break;
    }

    case ::d2::ScreenOpenMode::kBoth: {
      DrawLeftScreenBackground();
      DrawLeftScreenBorderFrame();
      DrawLeftScreenBackgroundRibbon();

      DrawRightScreenBackground();
      DrawRightScreenBorderFrame();
      DrawRightScreenBackgroundRibbon();

      break;
    }

    default: {
      ::mdc::error::ExitOnConstantMappingError(
          __FILEW__,
          __LINE__,
          static_cast<int>(screen_open_mode)
      );
    }
  }

  // Original code
  ::d2::d2client::DrawScreens();
}

} // namespace sgd2fr
