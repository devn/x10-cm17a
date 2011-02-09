#include "ruby.h"
#include "cm17a.h"

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

static X10_DEVICE fd;

static VALUE mX10;
static VALUE mCm17a;
static VALUE cCm17aController;
static VALUE eX10Error;
static VALUE symON;
static VALUE symOFF;
static VALUE symDIM;
static VALUE symBRIGHTEN;

/*
 * call-seq:
 *    X10::Cm17a::Controller.new()
 *    X10::Cm17a::Controller.new(device_name)
 *
 * Create a new CM17A controller on the serial device named
 * +device_name+.  If no device name is given, then a default device
 * will be selected (<tt>/dev/ttyS0</tt> for linux and <tt>COM1</tt>
 * for windows).
 */
static VALUE cm17a_init(int argc, VALUE *argv, VALUE self)
{
    const char * device_name = DEFAULT_SERIAL_DEVICE;
    if (argc > 0)
	device_name = StringValuePtr(argv[0]);
    fd = cm17a_open_device(device_name);
    if (fd == INVALID_X10_DEVICE)
	rb_raise(eX10Error, "Unable to open cm17a device '%s'", device_name);
    return self;
}

/*
 * call-seq:
 *    controller.command(house, unit, command_code, steps)
 *
 * Send a command to the CM17A controller.  The X10 unit to get the
 * address is given by the +house+ code (0-15) and the +unit+ code
 * (0-15).  The command must be one of the following constants:
 *
 * * <tt>:on</tt> -- Turn the device on.
 * * <tt>:off</tt> -- Turn the device off.
 * * <tt>:dim</tt> -- Dim the device by +steps+ steps.
 * * <tt>:brighten</tt> -- Brighten the device by +steps+ steps.
 *
 * Note that the unit code is ignored for the <tt>:brighten</tt> and
 * <tt>:dim</tt> commands.  The bright/dim commands will effect the
 * last addressed unit.
 */
static VALUE cm17a_ruby_command(
    VALUE self,
    VALUE rhouse,
    VALUE rdevice,
    VALUE rcommand,
    VALUE rsteps)
{
    int house = NUM2INT(rhouse);
    int device = NUM2INT(rdevice);
    int steps = NUM2INT(rsteps);
    int command;

    if (rcommand == symON)
	command = CM17A_ON;
    else if (rcommand == symOFF)
	command = CM17A_OFF;
    else if (rcommand == symDIM)
	command = CM17A_DIM;
    else if (rcommand == symBRIGHTEN)
	command = CM17A_BRIGHTEN;
    else
	command = NUM2INT(rcommand);
    
    cm17a_command(fd, house, device, command, steps);
    return Qnil;
}

/*
 * Close the controller device.
 */
static VALUE cm17a_close(VALUE self)
{
    cm17a_close_device(fd);
    return Qnil;
}

void Init_cm17a_api() 
{
    mX10 = rb_define_module("X10");
    mCm17a = rb_define_module_under(mX10, "Cm17a");
    cCm17aController = rb_define_class_under(mCm17a, "Controller", rb_cObject);

    eX10Error = rb_eval_string("X10::X10Error");

    symON = rb_eval_string(":on");
    symOFF = rb_eval_string(":off");
    symDIM = rb_eval_string(":dim");
    symBRIGHTEN = rb_eval_string(":brighten");

    rb_define_method(cCm17aController, "initialize", cm17a_init, -1);
    rb_define_method(cCm17aController, "close",      cm17a_close, 0);
    rb_define_method(cCm17aController, "command",    cm17a_ruby_command, 4);
}
