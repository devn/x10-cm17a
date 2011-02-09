/* 
 * Modification for dual Unix/Windows compatibility:
 *   Copyright 2004 by Jim Weirich (jim@weirichhouse.org)
 *   All rights reserved.
 *
 *   See the MIT-LICENSE file included with the distribution for
 *   details on redistribution rights.
 *
 * This cm17a driver file is a merging of two separate cm17a drivers,
 * one for Linux by Matt Armstrong and the other for Windows by Adam
 * Briggs. Both Matt and Adam have licensed their code so that it can
 * be modified and reused as long as the relevent copyrights notices,
 * conditions and disclaimers are intact.
 *
 * So first we have Matt's copyright notice for the Linux code:
 *
 *    Copyright (C) 1999, 2002 Matt Armstrong
 *    All rights reserved.
 *
 *    Redistribution and use in source and binary forms, with or
 *    without modification, are permitted provided that the
 *    following conditions are met:
 *
 *    1. Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *
 *    2. Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS
 *    IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *    ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE
 *    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 *    OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 *    OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 *    TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 *    OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 *    OF SUCH DAMAGE.
 *
 * Next we have Adam's copyright for the Windows code.
 *
 *    This code is (c)2000, Adam Briggs. All Rights Reserved.
 *  
 *    Permission to use, copy, and distribute this software and its
 *    documentation for any purpose and without fee is hereby granted,
 *    provided that the above copyright notice appear in all copies
 *    and that both that copyright notice and this permission notice
 *    appear in supporting documentation, and that the name Adam
 *    Briggs not be used in advertising or publicity pertaining to
 *    distribution of the software without specific, written prior
 *    permission.
 *  
 *                              *** DISCLAIMER ***
 *   
 *    ADAM BRIGGS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS
 *    SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 *    AND FITNESS, IN NO EVENT SHALL ADAM BRIGGS BE LIABLE FOR ANY
 *    SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 *    IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTUOUS ACTION,
 *    ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
 *    THIS SOFTWARE.
 *                                     ***
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef CM17A_WIN32

#include <windows.h>
#include <stdio.h>

#else

#include <stdio.h>
#include <stddef.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

#if HAVE_FCNTL_H
#include <fcntl.h>
#endif
#if HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif
#if HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#if HAVE_TERMIO_H
#include <termio.h>
#endif

#endif

#include "cm17a.h"

/* The house code is the high nibble of the command data. */
unsigned short house_codes[16] = {
    0x6000,			/* A */
    0x7000,			/* B */
    0x4000,			/* C */
    0x5000,			/* D */
    0x8000,			/* E */
    0x9000,			/* F */
    0xA000,			/* G */
    0xB000,			/* H */
    0xE000,			/* I */
    0xF000,			/* J */
    0xC000,			/* K */
    0xD000,			/* L */
    0x0000,			/* M */
    0x1000,			/* N */
    0x2000,			/* O */
    0x3000,			/* P */
};

/* The device codes are described by bits 3, 4, 6, and 10. */
#define DEVICE_BITS 0x0458
unsigned short device_codes[16] = {
    0x0000,			/* 1 */
    0x0010,			/* 2 */
    0x0008,			/* 3 */
    0x0018,			/* 4 */
    0x0040,			/* 5 */
    0x0050,			/* 6 */
    0x0048,			/* 7 */
    0x0058,			/* 8 */
    0x0400,			/* 9 */
    0x0410,			/* 10 */
    0x0408,			/* 11 */
    0x0418,			/* 12 */
    0x0440,			/* 13 */
    0x0450,			/* 14 */
    0x0448,			/* 15 */
    0x0458,			/* 16 */
};

/* The command codes are described by bits 1, 2, 5, 7, 8, 9, and
   11. */
unsigned short command_codes[] = {
    0x0000,			/* CM17A_ON */
    0x0020,			/* CM17A_OFF */
    0x0088,			/* BRIGHT */
    0x0098,			/* CM17A_DIM */
};

#define ELEMENT_COUNT(a) (sizeof(a) / sizeof((a)[0]))

static unsigned short
cm17a_command_word(int house, 
		   int device, 
		   enum CM17A_COMMAND command)
{
    unsigned short retval;

    switch (command) {
    case CM17A_BRIGHTEN:
    case CM17A_DIM:
        retval = house_codes[house] | command_codes[command];
        break;

    default:
        retval = (house_codes[house] | 
                  device_codes[device] |
                  command_codes[command]);
        break;
    }

    return retval;
}

#ifdef CM17A_WIN32

#define X10_DEVICE HANDLE

/* delay between bits */
static void delay(void)
{
    Sleep(2);
}

/* delay between commands */
static void long_delay(void)
{
    Sleep(5);
}

static int cm17a_standby(X10_DEVICE fd)
{
    EscapeCommFunction(fd, SETDTR);
    EscapeCommFunction(fd, SETRTS);
    delay();
    return 0;
}

static int write_byte(X10_DEVICE fd, unsigned char byte)
{
    int theMask = 0x80 ;

    while (theMask) {
        if (byte & theMask) {
            EscapeCommFunction(fd, SETRTS);
            EscapeCommFunction(fd, CLRDTR);
        }
        else {
            EscapeCommFunction(fd, SETDTR);
            EscapeCommFunction(fd, CLRRTS);
        }
        delay();

        EscapeCommFunction(fd, SETDTR);
        EscapeCommFunction(fd, SETRTS);
        delay();

        theMask >>= 1;
    }
    return 0;
}

X10_DEVICE cm17a_open_device(const char * device_name) {
    X10_DEVICE fd = CreateFile(
        device_name,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_WRITE_THROUGH,
        NULL) ;
    return fd;
}

void cm17a_close_device(X10_DEVICE fd) {
    CloseHandle(fd);
}


#else

#define X10_DEVICE int

enum SIGNAL {
    RESET = 0,
    HIGH = TIOCM_RTS,
    LOW = TIOCM_DTR,
    STANDBY = (TIOCM_RTS | TIOCM_DTR)
};

static void
sleep_us(unsigned int nusecs)
{
    struct timeval tval;

    tval.tv_sec = nusecs / 1000000;
    tval.tv_usec = nusecs % 1000000;
    select(0, NULL, NULL, NULL, &tval);
}

static void delay(void)
{
    sleep_us(500);
}

static void long_delay(void)
{
    sleep_us(500000);
}

static int set(X10_DEVICE fd, enum SIGNAL signal)
{
    int lines = signal;
    int current;
    /* FIXME: we should just remember this in a static
       variable. */
    if (ioctl(fd, TIOCMGET, &current) != 0) {
        return -1;
    }
    current &= ~RESET;
    current |= signal;
    return ioctl(fd, TIOCMSET, &lines);
}

static int cm17a_standby(X10_DEVICE fd)
{
    if (set(fd, STANDBY) != 0) {
        return -1;
    }
    delay();
    return 0;
}

static int write_byte(X10_DEVICE fd, unsigned char byte)
{
    int mask = 0x80;
    do {
        enum SIGNAL signal = (byte & mask) ? HIGH : LOW;
        if (set(fd, signal) != 0) {
            return -1;
        }
        delay();
        if (set(fd, STANDBY) != 0) {
            return -1;
        }
        delay();
        mask >>= 1;
    } while (mask);
    return 0;
}

X10_DEVICE cm17a_open_device(const char * device_name) {
    X10_DEVICE fd = open(device_name, O_RDONLY | O_NDELAY);
    return (fd < 0) ? INVALID_X10_DEVICE : fd;
}

void cm17a_close_device(fd) {
    close(fd);
}

#endif

static int
write_stream(
    X10_DEVICE fd, 
    const unsigned char* data, 
    size_t byte_count) 
{
    int i;
    for (i = 0; i < byte_count; i++) {
        int ret = write_byte(fd, data[i]);
        if (ret) {
            return ret;
        }
    }
    return 0;
}

static int
cm17a_header(X10_DEVICE fd)
{
    /* Header bits taken straight from the CM17A spec. */
    const unsigned char header[] = {
        0xD5, 0xAA
    };
    return write_stream(fd, header, sizeof(header));
}

static int
footer(X10_DEVICE fd)
{
    /* Footer bits taken straight from the CM17A spec. */
    return write_byte(fd, 0xAD);
}

static int
cm17a_write_command(
    X10_DEVICE fd, 
    int house, 
    int device, 
    enum CM17A_COMMAND command)
{
    short command_word;

    command_word = cm17a_command_word(house, device, command);

    if (cm17a_standby(fd)) {
        return -1;
    }

    /* The cm17a seems to need another delay period after a
       standby to settle down after the reset. */
    delay();

    if (cm17a_header(fd) ||
        write_byte(fd, command_word >> 8) || 
        write_byte(fd, command_word) ||
        footer(fd)) {
        return -1;
    }

    long_delay();

    return 0;
}

void
cm17a_command(
    X10_DEVICE fd,
    int house,
    int device,
    enum CM17A_COMMAND cmd,
    int steps)
{
    switch (cmd) {
    case CM17A_ON:
    case CM17A_OFF:
        if (cm17a_write_command(fd, house, device, cmd) != 0) {
            fprintf(stderr, "Command failed.\n");
        }
        break;
    case CM17A_BRIGHTEN:
    case CM17A_DIM:
    {
        if (steps < 1 || steps > 6) {
            fprintf(stderr, 
                    "Invalid steps (%d).  Must be [1..6].\n", 
                    steps);
            return;
        }

        /* Turn the device we wish to control on first.  If we don't
           do this, the dim command gets handled by the last device
           that got turned on.  The cm17a dim/brighten command doesn't
           use a device code. */
	   
        if (cm17a_write_command(fd, house, device, CM17A_ON) != 0) {
            fprintf(stderr, "Command failed.\n");
        }

        while (--steps >= 0) {
            if (cm17a_write_command(fd, house, device, cmd)) {
                fprintf(stderr, "Command failed.\n");
            }
        }
        break;
    }
    default:
        fprintf(stderr, "Invalid flip operation\n");
    }
}
