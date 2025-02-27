/*

Author: Steve Wood <swood@cromulence.com>

Copyright (c) 2016 Cromulence LLC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include <libcgc.h>
#include "stdlib.h"


int receive_bytes (char *buffer, size_t count) 
{
size_t total;
size_t rxbytes;


    if (buffer == 0 ) {

        return -1;

    }
    
    total = 0;

    while(total < count)  {

        rxbytes = 0;

        if (receive(STDIN, buffer+total, count-total, &rxbytes)==0 ) {

            total += rxbytes;

        }
        else {

            return(-1);
        }

    }

    return 0;

}


// Buffered receive_until function.  Will return up to "limit" characters in "buffer", but
// note that buffer[limit] will have a null written so ensure the incoming buffer is limit+1 in size
size_t receive_until( char *buffer, char delim, size_t limit )
{
#define maxLen 512
    static char circ_buffer[maxLen];
    static int head = 0;
    static int end = 0;
    int maxRead;
    size_t len = 0;
    size_t rx = 0;

    while (1) {

        for ( ; head < end; ++head ) {

            if (*(circ_buffer+head) == delim ) {

                *buffer = 0;
                ++head;
                return len;
            }

            *buffer = *(circ_buffer+head);
            ++len;
            ++buffer;

            if (len == limit) {

                *buffer = 0;
                return len;
            }
        }

        // wrap around to the beginning of the circular buffer
        if ( head == maxLen ) {

            head = 0;
            end = 0;
        }

        if (end < head) {

            maxRead = head - end;
        }
        else {

            maxRead = maxLen - end;
        }

        if (receive(STDIN, (void *)circ_buffer + end, maxRead, &rx) != 0 ) 
            _terminate(-1);

        end += rx;

    } // while (1)

}
