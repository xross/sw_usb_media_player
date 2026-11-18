#ifndef APP_PACING_BUTTON_H_
#define APP_PACING_BUTTON_H_
#include "audio_dac.h"
#include "video_config.h"

#if APP_XUH_MSC_PLAYER_AUDIO_DAC && APP_XUH_MSC_PLAYER_PACING_BUTTON
#include <xcore/port.h>

typedef struct {
    port_t port;
    unsigned idle, switched, pending;
} pacing_button_t;

void pacing_button_init(pacing_button_t *button);
unsigned pacing_button_paced(const pacing_button_t *button);
/* current is the pacing state in force. Returns -1 for no change, otherwise
 * the requested pacing state (0 or 1). */
int pacing_button_change(pacing_button_t *button, unsigned active, unsigned current);
unsigned pacing_button_wake(const pacing_button_t *button, unsigned wake);
#endif
#endif
