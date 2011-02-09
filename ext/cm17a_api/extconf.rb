require 'mkmf'
require 'rbconfig'

have_header("fcntl.h")
have_header("sys/ioctl.h")
have_header("sys/time.h")
have_header("sys/types.h")
have_header("unistd.h")
have_header("termio.h")

if Config::CONFIG['arch'] =~ /win32/
  $defs.push "-DCM17A_WIN32"
  $defs.push '-DDEFAULT_SERIAL_DEVICE=\\"COM1\\"'
else
  $defs.push '-DDEFAULT_SERIAL_DEVICE=\\"/dev/ttyS0\\"'
end
create_makefile("cm17a_api")
