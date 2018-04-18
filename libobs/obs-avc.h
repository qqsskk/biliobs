#pragma once

#include "util/c99defs.h"

#ifdef __cplusplus
extern "C" {
#endif

struct encoder_packet;

enum {
	OBS_NAL_UNKNOWN   = 0,
	OBS_NAL_SLICE     = 1,
	OBS_NAL_SLICE_DPA = 2,
	OBS_NAL_SLICE_DPB = 3,
	OBS_NAL_SLICE_DPC = 4,
	OBS_NAL_SLICE_IDR = 5,
	OBS_NAL_SEI       = 6,
	OBS_NAL_SPS       = 7,
	OBS_NAL_PPS       = 8,
	OBS_NAL_AUD       = 9,
	OBS_NAL_FILLER    = 12,
};

enum {
	OBS_NAL_PRIORITY_DISPOSABLE = 0,
	OBS_NAL_PRIORITY_LOW        = 1,
	OBS_NAL_PRIORITY_HIGH       = 2,
	OBS_NAL_PRIORITY_HIGHEST    = 3,
};

/* Helpers for parsing AVC NAL units.  */

EXPORT bool obs_avc_keyframe(const uint8_t *data, size_t size);
EXPORT const uint8_t *obs_avc_find_startcode(const uint8_t *p,
		const uint8_t *end);
EXPORT void obs_parse_avc_packet(struct encoder_packet *avc_packet,
		const struct encoder_packet *src);
EXPORT size_t obs_parse_avc_header(uint8_t **header, const uint8_t *data,
		size_t size);

#ifdef __cplusplus
}
#endif
