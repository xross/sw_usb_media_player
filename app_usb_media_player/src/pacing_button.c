#include "pacing_button.h"
#if APP_XUH_MSC_PLAYER_AUDIO_DAC && APP_XUH_MSC_PLAYER_PACING_BUTTON
#include <xs1.h>
#include <xcore/hwtimer.h>
#include "audio_playback.h"

enum { BUTTON_SETTLE_TICKS = AUDIO_REFERENCE_HZ / 20 };

/* SW1: pacing is off while the button is held, held being the opposite of the
 * level read at start-up. A change within BUTTON_SETTLE_TICKS (50 ms) of the
 * last switch may be bounce, and is acted on once that time has passed, without
 * a busy wait. The reference timer wraps every 43 s, so only the time since the
 * last switch is compared, never an absolute deadline. */
void pacing_button_init(pacing_button_t *button)
{
    button->port = XS1_PORT_1H;
    port_enable(button->port);
    button->idle = port_peek(button->port) & 1;
    button->switched = get_reference_time() - BUTTON_SETTLE_TICKS;
    button->pending = 0;
    port_set_trigger_in_not_equal(button->port, button->idle);
}

unsigned pacing_button_paced(const pacing_button_t *button)
{
    return (port_peek(button->port) & 1) == button->idle;
}

int pacing_button_change(pacing_button_t *button, unsigned active, unsigned current)
{
    unsigned now = get_reference_time();
    if (now - button->switched < BUTTON_SETTLE_TICKS) {
        button->pending = 1;
        return -1;
    }
    button->pending = 0;
    unsigned paced = pacing_button_paced(button);
    if (!active || paced == current) return -1;
    button->switched = now;
    return (int)paced;
}

unsigned pacing_button_wake(const pacing_button_t *button, unsigned wake)
{
    unsigned settled = button->switched + BUTTON_SETTLE_TICKS;
    return button->pending && (int)(settled - wake) < 0 ? settled : wake;
}
#endif
