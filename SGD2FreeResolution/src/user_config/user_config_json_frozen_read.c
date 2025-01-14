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

#include "user_config_json_frozen.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mdc/error/exit_on_error.h>
#include <mdc/malloc/malloc.h>
#include <mdc/wchar_t/filew.h>
#include <mdc/wchar_t/wide_decoding.h>
#include "../../third_party/frozen/frozen.h"
#include "../sgd2mapi_extension/file/file_content.h"
#include "user_config_json_key_value.h"
#include "user_config_struct.h"
#include "user_config_value_default.h"

#define TO_WIDE_IMPL(str_lit) L##str_lit
#define TO_WIDE(str_lit) TO_WIDE_IMPL(str_lit)

static void ExitOnInvalidConfig(
    const wchar_t* key_path,
    const wchar_t* file_path,
    int line
) {
  Mdc_Error_ExitOnGeneralError(
      L"Error",
      L"Invalid JSON config value for \"%ls\". The program will "
          L"now close.",
      file_path,
      line,
      key_path
  );
}

static void ReadMetadataVersion(const char *str, int len, void *user_data) {
  struct UserConfig_Metadata_Version* version;
  int json_scanf_result;

  version = (struct UserConfig_Metadata_Version*) user_data;

  json_scanf_result = json_scanf(
      str,
      len,
      "{"
          CONFIG_METADATA_VERSION_MINOR_HIGH ": %u,"
          CONFIG_METADATA_VERSION_MAJOR_LOW ": %u,"
          CONFIG_METADATA_VERSION_MINOR_HIGH ": %u,"
          CONFIG_METADATA_VERSION_MINOR_LOW ": %u"
      "}",
      &version->major_high,
      &version->major_low,
      &version->minor_high,
      &version->minor_low
  );

  if (json_scanf_result < 4) {
    ExitOnInvalidConfig(
        TO_WIDE(CONFIG_MAIN) L"." TO_WIDE(CONFIG_METADATA) L"."
            TO_WIDE(CONFIG_METADATA_VERSION),
        __FILEW__,
        __LINE__
    );
  }
}

static void ReadMetadata(const char *str, int len, void *user_data) {
  struct UserConfig_Metadata* metadata;
  int json_scanf_result;

  metadata = (struct UserConfig_Metadata*) user_data;

  json_scanf_result = json_scanf(
      str,
      len,
      "{"
          CONFIG_METADATA_VERSION ": %M"
      "}",
      &ReadMetadataVersion,
      &metadata->version
  );

  if (json_scanf_result < 1) {
    ExitOnInvalidConfig(
        TO_WIDE(CONFIG_MAIN) L"." TO_WIDE(CONFIG_METADATA),
        __FILEW__,
        __LINE__
    );
  }
}

static int ReadCustomMpqPath(
    const char *str,
    int len,
    struct UserConfig* config
) {
  int json_scanf_result;
  const char* custom_mpq_path_utf8;

  size_t custom_mpq_path_length;

  json_scanf_result = json_scanf(
      str,
      len,
      "{ " CONFIG_CUSTOM_MPQ_PATH ": %Q }",
      &custom_mpq_path_utf8
  );

  if (json_scanf_result < 1) {
    ExitOnInvalidConfig(
        TO_WIDE(CONFIG_MAIN) L"." TO_WIDE(CONFIG_CUSTOM_MPQ_PATH),
        __FILEW__,
        __LINE__
    );

    goto return_bad;
  }

  custom_mpq_path_length = Mdc_Wide_DecodeUtf8Length(custom_mpq_path_utf8);
  config->custom_mpq_path = Mdc_malloc(
      (custom_mpq_path_length + 1) * sizeof(config->custom_mpq_path[0])
  );

  if (config->custom_mpq_path == NULL) {
    Mdc_Error_ExitOnMemoryAllocError(__FILEW__, __LINE__);
    goto free_custom_mpq_path_utf8;
  }

  Mdc_Wide_DecodeUtf8(
      config->custom_mpq_path,
      custom_mpq_path_utf8
  );

  free(custom_mpq_path_utf8);

  return json_scanf_result;

free_custom_mpq_path_utf8:
  free(custom_mpq_path_utf8);

return_bad:
  return 0;
}

static void ReadResolution(const char *str, int len, void *user_data) {
  struct GameResolution* resolution;
  int json_scanf_result;

  resolution = (struct GameResolution*) user_data;

  json_scanf_result = json_scanf(
      str,
      len,
      "{ width: %d, height: %d }",
      &resolution->width,
      &resolution->height
  );

  if (json_scanf_result < 2) {
    Mdc_Error_ExitOnGeneralError(
        L"Error",
        L"Invalid JSON config resolution. The program will now close.",
        __FILEW__,
        __LINE__
    );
  }
}

static void ReadIngameResolutions(const char *str, int len, void *user_data) {
  size_t i;

  struct UserConfig* config;
  struct json_token token;
  int json_scanf_result;

  struct GameResolution resolution;

  config = (struct UserConfig*) user_data;

  for (i = 0; json_scanf_array_elem(str, len, "", i, &token) > 0; i += 1) {
    json_scanf_result = json_scanf(
        token.ptr,
        token.len,
        "%M",
        &ReadResolution,
        &resolution
    );

    if (json_scanf_result < 1) {
      Mdc_Error_ExitOnGeneralError(
          L"Error",
          L"Invalid JSON config value for \"%ls[%u]\". The program "
              L"will now close.",
          __FILEW__,
          __LINE__,
          TO_WIDE(CONFIG_MAIN) L"." TO_WIDE(CONFIG_INGAME_RESOLUTIONS),
          i
      );

      goto return_bad;
    }
  }

  config->ingame_resolutions.count = i;
  config->ingame_resolutions.resolutions = Mdc_malloc(
      config->ingame_resolutions.count
          * sizeof(config->ingame_resolutions.resolutions[0])
  );

  if (config->ingame_resolutions.resolutions == NULL) {
    Mdc_Error_ExitOnMemoryAllocError(__FILEW__, __LINE__);
    goto return_bad;
  }

  for (i = 0; json_scanf_array_elem(str, len, "", i, &token) > 0; i += 1) {
    json_scanf_result = json_scanf(
        token.ptr,
        token.len,
        "%M",
        &ReadResolution,
        &config->ingame_resolutions.resolutions[i]
    );

    if (json_scanf_result < 1) {
      Mdc_Error_ExitOnGeneralError(
          L"Error",
          L"Invalid JSON config value for \"%ls[%u]\". The program "
              L"will now close.",
          __FILEW__,
          __LINE__,
          TO_WIDE(CONFIG_MAIN) L"." TO_WIDE(CONFIG_INGAME_RESOLUTIONS),
          i
      );

      return;
    }
  }

  return;

return_bad:
  return;
}

static void ReadConfig(const char *str, int len, void *user_data) {
  struct UserConfig* config;
  int json_scanf_result;

  config = (struct UserConfig*) user_data;

  json_scanf_result = json_scanf(
      str,
      len,
      "{"
          CONFIG_METADATA ": %M,"
          CONFIG_INGAME_RESOLUTION_MODE ": %u,"
          CONFIG_INGAME_RESOLUTIONS ": %M,"
          CONFIG_IS_ENABLE_SCREEN_BORDER_FRAME ": %B,"
          CONFIG_IS_USE_800_INTERFACE_BAR ": %B,"
          CONFIG_IS_USE_ORIGINAL_SCREEN_BORDER_FRAME ": %B,"
          CONFIG_MAIN_MENU_RESOLUTION ": %M"
      "}",
      &ReadMetadata,
      &config->metadata,
      &config->ingame_resolution_mode,
      &ReadIngameResolutions,
      config,
      &config->is_enable_screen_border_frame,
      &config->is_use_800_interface_bar,
      &config->is_use_original_screen_border_frame,
      &ReadResolution,
      &config->main_menu_resolution
  );

  /*
   * Custom MPQ Path has to be read in a separate function, due to %M
   * removing initial quotes, and the path requiring conversion to
   * wstring.
   */
  json_scanf_result += ReadCustomMpqPath(str, len, config);

  /* json_scanf does not count Ingame Resolution Mode. */
  json_scanf_result += 1;

  if (json_scanf_result < 8) {
    Mdc_Error_ExitOnGeneralError(
        L"Error",
        L"Invalid JSON config. The program will now close.%d",
        __FILEW__,
        __LINE__,
        json_scanf_result
    );
  }
}

/**
 * External
 */

void ConfigJsonFrozen_Read(struct UserConfig* config, const wchar_t* path) {
  size_t file_length;
  char* file_contents;

  file_length = Mapi_File_GetFileContentLength(path);
  file_contents = (char*)Mdc_malloc(file_length * sizeof(file_contents[0]));
  if (file_contents == NULL) {
    Mdc_Error_ExitOnMemoryAllocError(__FILEW__, __LINE__);
    goto return_bad;
  }
  Mapi_File_ReadFileContent(file_contents, file_length, path);

  json_scanf(
      file_contents,
      file_length,
      "{ " CONFIG_MAIN ": %M }",
      &ReadConfig,
      config
  );

  return;

return_bad:
  return;
}

void ConfigJsonFrozen_CleanUp(struct UserConfig* config) {
  Mdc_free(config->custom_mpq_path);
  Mdc_free(config->ingame_resolutions.resolutions);
}
