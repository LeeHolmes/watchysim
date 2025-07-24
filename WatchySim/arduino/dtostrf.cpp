#include <stdio.h>

char* dtostrf(double val, signed char width, unsigned char prec, char* sout) {
	char fmt[20];
	snprintf(fmt, sizeof(fmt), "%%%d.%df", width, prec);
	sprintf(sout, fmt, val);
	return sout;
}
