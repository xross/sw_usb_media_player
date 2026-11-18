#ifndef APP_REFERENCE_TIME_H_
#define APP_REFERENCE_TIME_H_

/* The supported XN targets use a fixed 100-MHz reference clock. These are
 * compile-time units, not application configuration options. */
enum {
    REFERENCE_TICKS_PER_SECOND = 100000000u,
    REFERENCE_TICKS_PER_MS = REFERENCE_TICKS_PER_SECOND / 1000u,
    REFERENCE_TICKS_PER_US = REFERENCE_TICKS_PER_SECOND / 1000000u
};

/* Keep unsigned, modulo-2^32 arithmetic for hardware-timer timestamps. */
static inline unsigned reference_ticks_from_ms(unsigned ms)
{
    return ms * REFERENCE_TICKS_PER_MS;
}

static inline unsigned reference_ticks_from_us(unsigned us)
{
    return us * REFERENCE_TICKS_PER_US;
}

static inline unsigned reference_ticks_to_ms(unsigned ticks)
{
    return ticks / REFERENCE_TICKS_PER_MS;
}

static inline unsigned reference_ticks_to_us(unsigned ticks)
{
    return ticks / REFERENCE_TICKS_PER_US;
}
#endif
