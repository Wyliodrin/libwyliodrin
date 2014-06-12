
#include <Arduino.h>
#include <IPAddress.h>

IPAddress::IPAddress()
{
	_sin.sin_addr.s_addr = htonl(INADDR_NONE);
}

IPAddress::IPAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
	_sin.sin_addr.s_addr = ( ((uint32_t)fourth_octet)<<24 | ((uint32_t)third_octet)<<16 | ((uint32_t)second_octet)<<8 | ((uint32_t)first_octet) );
}

IPAddress::IPAddress(uint32_t address)
{
	_sin.sin_addr.s_addr = htonl(address);
}

IPAddress::IPAddress(const uint8_t *address)
{
	_sin.sin_addr.s_addr = inet_addr((char*)address);//adr);
}

IPAddress& IPAddress::operator=(const uint8_t *address)
{
	_sin.sin_addr.s_addr = inet_addr((char*)address);
	return *this;
}

IPAddress& IPAddress::operator=(uint32_t address)
{
	_sin.sin_addr.s_addr = htonl(address);
	return *this;
}

IPAddress& IPAddress::operator=(struct sockaddr_in *psin)
{
	_sin.sin_addr.s_addr = psin->sin_addr.s_addr;
	return *this;
}

bool IPAddress::operator==(const uint8_t* address)
{
	return _sin.sin_addr.s_addr == inet_addr((char*)address);
}

size_t IPAddress::printTo(Print& p) const
{
	size_t n = 0;
	char buf[0x20];

	n = snprintf(buf, sizeof(buf), "%3d.%3d.%3d.%3d",
		(_sin.sin_addr.s_addr&0x000000FF),
		(_sin.sin_addr.s_addr&0x0000FF00)>>8,
		(_sin.sin_addr.s_addr&0x00FF0000)>>16,
	    	(_sin.sin_addr.s_addr&0xFF000000)>>24
	    	);

	p.print(buf);
	return n;
}

