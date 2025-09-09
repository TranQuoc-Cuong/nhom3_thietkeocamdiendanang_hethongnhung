#include "Ringbuffer.h"

int8_t ring_buff_push(RingBuff *ring_buff, uint8_t data)
{
	uint16_t next;
	
	next = ring_buff->head + 1;
	if (next >= ring_buff->maxlen)
		next = 0;
	
	if (next == ring_buff->tail)
		return -1;
			
	ring_buff->buffer[ring_buff->head] = data;
	ring_buff->head = next;
	return 0;
}

int8_t ring_buff_pop(RingBuff *c, uint8_t *data)
{
	int next;
	
	if(c->head == c->tail)
		return -1;
	
	next = c->tail + 1;
	if(next >= c->maxlen)
		next = 0;
	
	*data = c->buffer[c->tail];
	c->tail = next;
	return 0;
}

uint16_t ring_buff_available(RingBuff *ring_buff)
{
	if(ring_buff->head < ring_buff->tail)
		return ring_buff->maxlen - (ring_buff->tail - ring_buff->head);
	return (ring_buff->head - ring_buff->tail);
}

void ring_buff_init(RingBuff *ring_buff, uint8_t *buff, uint16_t len)
{
	ring_buff->buffer = buff;
	ring_buff->head = 0;
	ring_buff->tail = 0;
	ring_buff->maxlen = len;
}
