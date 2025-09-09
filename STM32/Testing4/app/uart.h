#ifndef UART_H
#define UART_H
#include <stdint.h>

void receive_rx(uint8_t data_rx);
void uart_init(void);
uint16_t uart_available(void);
uint8_t uart_read(void);

#endif
