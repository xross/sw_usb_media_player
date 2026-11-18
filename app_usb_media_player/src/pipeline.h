#ifndef APP_PIPELINE_H_
#define APP_PIPELINE_H_

#include <xccompat.h>
#include "video_config.h"

#define PIPELINE_STOP (~0u)

/* Development-only per-buffer integrity checks during transmit/render.
 * Disabled for timing runs; enable with -DPIPELINE_VERIFY=1. */
#ifndef PIPELINE_VERIFY
#define PIPELINE_VERIFY 0
#endif

enum {
    PIPELINE_FRAMES,
    PIPELINE_FIRST_COMPLETE,
    PIPELINE_LAST_COMPLETE,
    /* Stage totals below are microseconds, not raw timer ticks. */
    PIPELINE_TRANSFER_TICKS,
    PIPELINE_RECEIVE_TICKS, /* MPEG YUV strip reception, including channel waits. */
    /* MPEG: conversion only. JPEG: combined reception and conversion. */
    PIPELINE_CONVERT_TICKS,
    PIPELINE_RENDER_TICKS,
    /* MPEG receiver blocked on the previous LCD frame, excluding startup/drain. */
    PIPELINE_RENDER_WAIT_TICKS,
    PIPELINE_DECODE_TICKS,
    PIPELINE_ERRORS,
    PIPELINE_STATS_WORDS
};
/* Fixed protocol extension; mpeg_pipeline.h checks its enum against this size.
 * These counts are compile-time constants for packet/array sizes. */
enum { PIPELINE_DECODER_STATS_WORDS = 8,
       PIPELINE_LOADER_STATS_WORDS = PIPELINE_STATS_WORDS + PIPELINE_DECODER_STATS_WORDS };

#if MSC_VIDEO_PIPELINE && !defined(__XC__)
#include <xs1.h>
#include <xcore/chanend.h>

/* END releases every route. Reverse messages explicitly return ownership.
 * A producer may fill the alternate slot while its consumer owns this slot,
 * but must receive that consumer's completion before publishing another slot.
 * Buffer pointers are used only by tasks on the same tile. */
static inline void pipeline_send(chanend c, const unsigned *words, unsigned count)
{
    /* Raw chanend instructions have no memory clobber: publish buffer writes
     * before transferring ownership, and acquire it again on return. */
    asm volatile("" ::: "memory");
    for (unsigned i = 0; i < count; ++i) chanend_out_word(c, words[i]);
    chanend_out_control_token(c, XS1_CT_END);
    asm volatile("" ::: "memory");
}

static inline void pipeline_receive(chanend c, unsigned *words, unsigned count)
{
    asm volatile("" ::: "memory");
    for (unsigned i = 0; i < count; ++i) words[i] = chanend_in_word(c);
    chanend_check_control_token(c, XS1_CT_END);
    asm volatile("" ::: "memory");
}
#endif
#endif
