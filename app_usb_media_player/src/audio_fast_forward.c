#include "audio_fast_forward.h"
#if APP_XUH_MSC_PLAYER_AUDIO_DAC
#include "audio_dac_queue.h"
#include "audio_playback.h"

static const unsigned fractional_bits = 16;  /* Q16 source-frame positions. */
static const unsigned initial_rate_numerator = 3, initial_rate_denominator = 2;
static const unsigned maximum_rate = 4;
static const unsigned arrival_window_ticks = 250 * REFERENCE_TICKS_PER_MS;
static const unsigned target_rate_numerator = 15, target_rate_denominator = 16;
static const unsigned smoothing_weight = 4; /* New reading gets one quarter. */

void audio_fast_forward_reset(audio_fast_forward_t *state, unsigned now, unsigned decoded_frames)
{
    const unsigned unity_step = 1u << fractional_bits;
    state->step = unity_step * initial_rate_numerator / initial_rate_denominator;
    state->window_start = now;
    state->window_frames = decoded_frames;
}

void audio_fast_forward_follow(audio_fast_forward_t *state, unsigned now, unsigned decoded_frames)
{
    unsigned elapsed = now - state->window_start;
    if (elapsed < arrival_window_ticks) return;
    const unsigned unity_step = 1u << fractional_bits;
    const unsigned maximum_step = unity_step * maximum_rate;
    unsigned long long played = (unsigned long long)elapsed * AUDIO_DAC_RATE / AUDIO_REFERENCE_HZ;
    /* Aim about 6% slow, favouring a full queue over gaps and silence. */
    unsigned long long rate = (unsigned long long)(decoded_frames - state->window_frames) *
                              unity_step * target_rate_numerator / target_rate_denominator / played;
    if (rate < unity_step) rate = unity_step;
    if (rate > maximum_step) rate = maximum_step;
    state->step = ((smoothing_weight - 1) * state->step + (unsigned)rate) / smoothing_weight;
    state->window_start = now;
    state->window_frames = decoded_frames;
}

unsigned audio_fast_forward_write(const audio_fast_forward_t *state,
                                 const mpeg_audio_sample_t *block, unsigned *position)
{
    enum { OUTPUT_CHUNK_FRAMES = 32 }; /* Compile-time array bound. */
    int32_t chunk[2 * OUTPUT_CHUNK_FRAMES];
    for (;;) {
        unsigned at = *position, count = 0;
        while (count < OUTPUT_CHUNK_FRAMES && (at >> fractional_bits) < MPEG_AUDIO_FRAME_SAMPLES) {
            chunk[2 * count] = block[2 * (at >> fractional_bits)];
            chunk[2 * count + 1] = block[2 * (at >> fractional_bits) + 1];
            ++count;
            at += state->step;
        }
        if (!count) return 0;
        /* Not kept to the clock: after a gap, play on rather than drop. */
        unsigned taken = audio_dac_write_now(chunk, count);
        *position += taken * state->step;
        if (taken < count) break;
    }
    unsigned frame = *position >> fractional_bits;
    return frame < MPEG_AUDIO_FRAME_SAMPLES ? MPEG_AUDIO_FRAME_SAMPLES - frame : 0;
}
#endif
