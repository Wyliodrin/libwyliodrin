
#ifndef __TTYUART_H__
#define __TTYUART_H__

#include "HardwareSerial.h"
#include "RingBuffer.h"

#include <pthread.h>

class TTYUARTClass : public HardwareSerial
{
private:

	// Data
	RingBuffer *_rx_buffer ;
	int _dwId;				// For mux identification - see variant.cpp for index resolution
	int _tty_fd;
	int _pipe_tx_rx[2];
	char * _tty_name;
	uint32_t _dwBaudRate;

	// Use threads to support the model of available/peek the Serial class requires
	pthread_t _thread;
	pthread_mutex_t _mutex;
	pthread_barrier_t _barrier;

public:
	TTYUARTClass( RingBuffer* pRx_buffer, uint32_t dwId ) ;
	virtual ~TTYUARTClass();

	static void * TTYIrqHandler(void * pargs);	// static has no implicit this
	int init_tty( char * tty_name );
	void begin( const uint32_t dwBaudRate ) ;
	void end( void ) ;
	int available( void ) ;
	int peek( void ) ;
	int read( void ) ;
	void flush( void ) ;
	size_t write( const uint8_t c ) ;
	using Print::write ; // pull in write(str) and write(buf, size) from Print

	operator bool() { return true; }; // UART always active
};

#endif /* __TTYUART_H__ */
