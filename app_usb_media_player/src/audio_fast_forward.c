#include "audio_fast_forward.h"
#if APP_XUH_MSC_PLAYER_AUDIO_DAC
#include "audio_dac_queue.h"
#include "audio_playback.h"

enum { STEP_ONE = 65536, STEP_START = STEP_ONE * 3 / 2, STEP_MAX = STEP_ONE * 4,
       STEP_WINDOW_TICKS = AUDIO_REFERENCE_HZ / 4 };

void audio_fast_forward_reset(audio_fast_forward_t *state, unsigned now, unsigned decoded_frames)
{
    state->step = STEP_START;
    state->window_start = now;
    state->window_frames = decoded_frames;
}

void audio_fast_forward_follow(audio_fast_forward_t *state, unsigned now, unsigned decoded_frames)
{
    unsigned elapsed = now - state->window_start;
    if (elapsed < STEP_WINDOW_TICKS) return;
    unsigned long long played = (unsigned long long)elapsed * AUDIO_DAC_RATE / AUDIO_REFERENCE_HZ;
    /* Aim about 6% slow, favouring a full queue over gaps and silence. */
    unsigned long long rate = (unsigned long long)(decoded_frames - state->window_frames) *
                              STEP_ONE * 15 / 16 / played;
    if (rate < STEP_ONE) rate = STEP_ONE;
    if (rate > STEP_MAX) rate = STEP_MAX;
    state->step = (3 * state->step + (unsigned)rate) / 4;
    state->window_start = now;
    state->window_frames = decoded_frames;
}

unsigned audio_fast_forward_write(const audio_fast_forward_t *state,
                                 const mpeg_audio_sample_t *block, unsigned *position)
{
    enum { CHUNK = 32 };
    int32_t chunk[2 * CHUNK];
    for (;;) {
        unsigned at = *position, count = 0;
        while (count < CHUNK && (at >> 16) < MPEG_AUDIO_FRAME_SAMPLES) {
            chunk[2 * count] = block[2 * (at >> 16)];
            chunk[2 * count + 1] = block[2 * (at >> 16) + 1];
            ++count;
            at += state->step;
        }
        if (!count) return 0;
        /* Not kept to the clock: after a gap, play on rather than drop. */
        unsigned taken = audio_dac_write_now(chunk, count);
        *position += taken * state->step;
        if (taken < count) break;
    }
    unsigned frame = *position >> 16;
    return frame < MPEG_AUDIO_FRAME_SAMPLES ? MPEG_AUDIO_FRAME_SAMPLES - frame : 0;
}
#endif
