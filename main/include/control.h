#ifndef CONTROL_H
#define CONTROL_H

void check_system_handler(void *arg);
void check_relay(float average_temp);
void relay_deinit(void);

#endif // CONTROL_H