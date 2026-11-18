#ifndef APP_AUDIO_FAST_FORWARD_H_
#define APP_AUDIO_FAST_FORWARD_H_
#include "audio_dac.h"

#if APP_XUH_MSC_PLAYER_AUDIO_DAC
#include "mpeg_audio.h"

/* Demo-only decimation: follow the bursty input rate and play faster by
 * retaining evenly spaced stereo frames. This deliberately raises pitch. */
typedef struct {
    unsigned step, window_start, window_frames;
} audio_fast_forward_t;

void audio_fast_forward_reset(audio_fast_forward_t *state, unsigned now, unsigned decoded_frames);
void audio_fast_forward_follow(audio_fast_forward_t *state, unsigned now, unsigned decoded_frames);
/* position is in Q16 source frames. Return source frames still to consume. */
unsigned audio_fast_forward_write(const audio_fast_forward_t *state,
                                 const mpeg_audio_sample_t *block, unsigned *position);
#endif
#endif
