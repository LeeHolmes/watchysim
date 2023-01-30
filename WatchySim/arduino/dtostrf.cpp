#include <stdio.h>

char* dtostrf(double val, signed char width, unsigned char prec, char* sout) {
	char fmt[20];
	sprintf_s(fmt, 8, "%%%d.%df", width, prec);
	sprintf_s(sout, 20, fmt, val);
	return sout;
}
