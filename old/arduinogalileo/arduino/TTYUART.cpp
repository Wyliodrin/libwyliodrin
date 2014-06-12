/*
 * Copyright(c) 2013 Intel Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Contact Information:
 * Intel Corporation
 */

#include "Mux.h"
#include "TTYUART.h"
#undef B0		// termios.h has a B0 (baud rate 0) definition - which collides with arduino definition of B0 00000000 !

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#define __TTYUART_IDX_TX 1
#define __TTYUART_IDX_RX 0

// Constructors ////////////////////////////////////////////////////////////////

TTYUARTClass::TTYUARTClass( RingBuffer* pRx_buffer, uint32_t dwId  )
{
	_rx_buffer = pRx_buffer;
	_tty_fd = -1;
	_pipe_tx_rx[__TTYUART_IDX_TX] = -1;
	_pipe_tx_rx[__TTYUART_IDX_RX] = -1;
	_dwId = dwId;
	pthread_mutex_init(&_mutex, 0);
}

TTYUARTClass::~TTYUARTClass()
{
	if (_tty_fd != -1){
		close(_tty_fd);
		_tty_fd = -1;
	}

	if (_pipe_tx_rx[__TTYUART_IDX_TX] != -1){
		close(_pipe_tx_rx[__TTYUART_IDX_TX]);
		_pipe_tx_rx[__TTYUART_IDX_TX] = -1;
	}

	if (_pipe_tx_rx[__TTYUART_IDX_RX] != -1){
		close(_pipe_tx_rx[__TTYUART_IDX_RX]);
		_pipe_tx_rx[__TTYUART_IDX_RX] = -1;
	}
	pthread_mutex_destroy(&_mutex);
}

// Private methods /////////////////////////////////////////////////////////////

static ssize_t _read(int fd, void *buf, size_t count)
{
	extern ssize_t read(int fd, void *buf, size_t count);
	return read(fd, buf, count);
}

static ssize_t _write(int fd, const void *buf, size_t count)
{
	extern ssize_t write(int fd, const void *buf, size_t count);
	return write(fd, buf, count);
}

void * TTYUARTClass::TTYIrqHandler(void * pargs)
{
	TTYUARTClass * pTTYUARTClass = (TTYUARTClass*)pargs;
	fd_set fdset;
	char rx;
	unsigned int spin = 1;
	int ret = 0, max = pTTYUARTClass->_pipe_tx_rx[__TTYUART_IDX_RX];
	extern int errno;

	pthread_barrier_wait(&pTTYUARTClass->_barrier);

	/* Get max */
	if ( max < pTTYUARTClass->_tty_fd )
		max = pTTYUARTClass->_tty_fd;

	while(spin == 1){
		/* Init flag */
		rx = 0;

		/* zero */
		FD_ZERO(&fdset);

		/* Add elements */
		FD_SET(pTTYUARTClass->_tty_fd, &fdset);
		FD_SET(pTTYUARTClass->_pipe_tx_rx[__TTYUART_IDX_RX] , &fdset);

		/* Select on the FD set - infinite timeout */
		ret = select(1 + max, &fdset, 0, 0, NULL);

		/* Receive bytes */
		switch(ret){
			case -1:
				fprintf(stderr, "critical fault during select errno=%d", errno);
				spin = 0;
				break;
			case 0:
				/* timeout */
				break;
			default:
				/* Process data */
				if(FD_ISSET(pTTYUARTClass->_tty_fd, &fdset)){

					do {
						/* Read in frame */
						ret = _read(pTTYUARTClass->_tty_fd, &rx, 1);
						if(ret < 0){
							if((ret =! EAGAIN) && (ret != EWOULDBLOCK)){
								/* Critical - exit out */
								spin = 0;
							}
						}else{
							pTTYUARTClass->_rx_buffer->store_char( rx ) ;
						}
					}while(ret > 0);
				}

				// signal sent to end spin
				if(FD_ISSET(pTTYUARTClass->_pipe_tx_rx[__TTYUART_IDX_RX] , &fdset)){
					spin = 0;
				}
				break;
		}
	}

	/* Exit out */
	pthread_exit(NULL);

	return NULL;
}

// Public Methods //////////////////////////////////////////////////////////////
int TTYUARTClass::init_tty( char * tty_name )
{
	extern int errno;
	int ret;

	if(tty_name == NULL)
		return -EINVAL;

	// Open handle to tty
	_tty_fd = open(tty_name, O_RDWR);
	if (_tty_fd < 0){
		fprintf(stderr, "unable to open %s rdwr\n", tty_name);
		fflush(stdout);
		return errno;
	}

	// Open fifo
	ret = pipe2(_pipe_tx_rx, O_NONBLOCK);
	if (ret < 0){
		fprintf(stderr, "error making pipe!\n");
		return errno;
	}

	return 0;
}

void TTYUARTClass::begin( const uint32_t dwBaudRate )
{
	struct termios newtios, oldtermios;

	(void)muxSelectUart(_dwId);	// Mux pins as appropriate for this tty

	switch(dwBaudRate){
		case 50:
			_dwBaudRate = B50;
			break;
		case 75:
			_dwBaudRate = B75;
			break;
		case 110:
			_dwBaudRate = B110;
			break;
		case 134:
			_dwBaudRate = B134;
			break;
		case 150:
			_dwBaudRate = B150;
			break;
		case 200:
			_dwBaudRate = B200;
			break;
		case 300:
			_dwBaudRate = B300;
			break;
		case 600:
			_dwBaudRate = B600;
			break;
		case 1200:
			_dwBaudRate = B1200;
			break;
		case 1800:
			_dwBaudRate = B1800;
			break;
		case 2400:
			_dwBaudRate = B2400;
			break;
		case 4800:
			_dwBaudRate = B4800;
			break;
		case 9600:
			_dwBaudRate = B9600;
			break;
		case 19200:
			_dwBaudRate = B19200;
			break;
		case 38400:
			_dwBaudRate = B38400;
			break;
		case 57600:
			_dwBaudRate = B57600;
			break;
		case 115200:
			_dwBaudRate = B115200;
			break;
		case 230400:
			_dwBaudRate = B230400;
			break;
		case 460800:
			_dwBaudRate = B460800;
			break;
		case 500000:
			_dwBaudRate = B500000;
			break;
		case B576000:
			_dwBaudRate = B576000;
			break;
		case B921600:
			_dwBaudRate = B921600;
			break;
		case B1000000:
			_dwBaudRate = B1000000;
			break;
		case B1152000:
			_dwBaudRate = B1152000;
			break;
		case B1500000:
			_dwBaudRate = B1500000;
			break;
		case B2000000:
			_dwBaudRate = B2000000;
			break;
		case B2500000:
			_dwBaudRate = B2500000;
			break;
		case B3000000:
			_dwBaudRate = B3000000;
			break;
		case B3500000:
			_dwBaudRate = B3500000;
			break;
		case B4000000:
			_dwBaudRate = B4000000;
			break;
		default:
			_dwBaudRate = B115200;
			break;
	}

	// TODO: _dwBaudRate
	//	newtios.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;

	tcgetattr(_tty_fd, &oldtermios); /* save current port settings */

	bzero(&newtios, sizeof(newtios));

	//newtios.c_cflag |= _dwBaudRate | CRTSCTS | CS8 | CLOCAL | CREAD;
	newtios.c_cflag = oldtermios.c_cflag;
	newtios.c_cflag &= ~CBAUD;
	newtios.c_cflag |= _dwBaudRate;
	newtios.c_iflag = IGNPAR;
	newtios.c_oflag = 0;

	/* set input mode (non-canonical, no echo,...) */
	newtios.c_lflag = 0;

	newtios.c_cc[VTIME]    = 0;   /* inter-character timer unused */
	newtios.c_cc[VMIN]     = 1;   /* blocking read until 1 chars received */

	tcflush(_tty_fd, TCIFLUSH);
	tcsetattr(_tty_fd, TCSANOW, &newtios);

	pthread_barrier_init(&_barrier, 0, 2);

	// Initiate IRQ handler
	pthread_create(&_thread, NULL, &TTYIrqHandler, this);

	// sync startup
	pthread_barrier_wait(&_barrier);
}

void TTYUARTClass::end( void )
{
	char c;

	// Write to the fifo_fd_wr to signal termination to 'IRQ' thread
	_write(_pipe_tx_rx[__TTYUART_IDX_TX], &c, 1);

	// pthread_join to sync
	pthread_join(_thread, NULL);

	// destroy barrier
	pthread_barrier_destroy(&_barrier);
}

int TTYUARTClass::available( void )
{
	uint32_t ret;

	pthread_mutex_lock(&_mutex);
	ret = (uint32_t)(SERIAL_BUFFER_SIZE + _rx_buffer->_iHead - _rx_buffer->_iTail) % SERIAL_BUFFER_SIZE ;
	pthread_mutex_unlock(&_mutex);

	return ret;
}

int TTYUARTClass::peek( void )
{
	int ret;

	pthread_mutex_lock(&_mutex);
	if ( _rx_buffer->_iHead == _rx_buffer->_iTail )
		ret = -1;
	else
		ret = _rx_buffer->_aucBuffer[_rx_buffer->_iTail] ;
	pthread_mutex_unlock(&_mutex);

	return ret;
}

// Read a sigle byte from TTY device non-blocking - use poll() to determine RX availability
int TTYUARTClass::read( void )
{
	// if the head isn't ahead of the tail, we don't have any characters
	int ret;

	pthread_mutex_lock(&_mutex);
	if ( _rx_buffer->_iHead == _rx_buffer->_iTail ){
		ret = -1;
	}else{
		ret = _rx_buffer->_aucBuffer[_rx_buffer->_iTail] ;
		_rx_buffer->_iTail = (unsigned int)(_rx_buffer->_iTail + 1) % SERIAL_BUFFER_SIZE ;
	}
	pthread_mutex_unlock(&_mutex);

	return ret ;
}

void TTYUARTClass::flush( void )
{
// Wait for transmission to complete
//while ((_pUsart->US_CSR & US_CSR_TXRDY) != US_CSR_TXRDY)
//	;
}

// Write to TTY in blocking mode
size_t TTYUARTClass::write( const uint8_t c )
{
	return _write(_tty_fd, &c, 1);
}
