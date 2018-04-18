#pragma once

/** Maximum number of source channels for output and per display */
#define MAX_CHANNELS 64

#define OBS_ALIGN_CENTER (0)
#define OBS_ALIGN_LEFT   (1<<0)
#define OBS_ALIGN_RIGHT  (1<<1)
#define OBS_ALIGN_TOP    (1<<2)
#define OBS_ALIGN_BOTTOM (1<<3)

#define MODULE_SUCCESS             0
#define MODULE_ERROR              -1
#define MODULE_FILE_NOT_FOUND     -2
#define MODULE_MISSING_EXPORTS    -3
#define MODULE_INCOMPATIBLE_VER   -4

#define OBS_OUTPUT_SUCCESS         0
#define OBS_OUTPUT_BAD_PATH       -1
#define OBS_OUTPUT_CONNECT_FAILED -2
#define OBS_OUTPUT_INVALID_STREAM -3
#define OBS_OUTPUT_ERROR          -4
#define OBS_OUTPUT_DISCONNECTED   -5
#define OBS_OUTPUT_UNSUPPORTED    -6
#define OBS_OUTPUT_NO_SPACE       -7

#define OBS_VIDEO_SUCCESS           0
#define OBS_VIDEO_FAIL             -1
#define OBS_VIDEO_NOT_SUPPORTED    -2
#define OBS_VIDEO_INVALID_PARAM    -3
#define OBS_VIDEO_CURRENTLY_ACTIVE -4
#define OBS_VIDEO_MODULE_NOT_FOUND -5
