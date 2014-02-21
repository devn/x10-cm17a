= CM17A Firecracker X10 Interface Code

The CM17A Firecracker X10 module is a simple to use X10 controller
that attaches to the serial port of your computer.  It communicates
with a wireless transceiver, sending it X10 commands that are further
sent to other X10 modules via your household power lines.  The
Firecracker controller allows you to control on/off devices throughout
your house.

The x10-cm17a allows Ruby programs to send simple on/off and
dim/bright commands to any device controlled by the CM17A controller.

== Download and Installation

=== Gem Installation

If you install using RubyGems, all you need to do is:

  gem install x10-cm17a    # may require root privilege

Be aware that the x10-cm17a software contains a C extension and
requires you to have a C build environment to install from
source. This is not an issue on unix systems, but a C build
environment is not part of the default Windows installations.  See
http://rubygarden.org/ruby?WindowsCompiler for more information about
compiling C extensions on windows.

I hope to have a binary gem for windows soon.

=== TGZ Installation

* Download the TGZ file from the x10-cm17a files area (http://rubyforge.org/project/showfiles.php?group_id=497).
* Unpack the TGZ file into an appropriate directory.
* CD into that directory and type:

    ruby setup.rb config
    ruby setup.rb setup
    ruby setup.rb install   # may require root privilege

Installing from a TGZ also requires a C build environment.  See the
notes above in the gem install section for more details.

== Using the CM17A Library

Using the x10-cm17a library is very easy.  Here is the basic example:

  require 'x10/cm17a'

  lamp = X10.device('a1')  # Create an X10 device
                           # ... at X10 address 'a1'
  lamp.on                  # Turn the device on
  lamp.step -5             # Dim the lamp by 5 steps
  lamp.off                 # Turn the lamp off

That's all there to it!

== Explicit X10 Controller Selection

By default, the X10 system will find the controller loaded by the
require statement and use that.  If there are more than one X10
controller class loaded in the system, or if you wish to construct the
controller with arguments, then you need to explicitly create the X10
controller.

* If your CM17A module is on <code>/dev/ttyS1</code> (instead of the
  default <code>/dev/ttyS0</code>), you may do the following:

    X10.controller = X10::Cm17a::Controller.new("/dev/ttyS1")

* If you wish to use the remote DRb driver to access a CM17A module on
  a remote computer, you may use the following:

    X10.controller =
      X10::Cm17aRemote::Controller.new("druby://remote_host:7777")

== License

Except as noted below, this software is made available under an
MIT-style open source license. See the file MIT-LICENSE included in
the distribution for details.

=== Additional Licence for cm17a.h and cm17a.c

The C code for x10-cm17a was, in part, derived from the Linux FlipIt
command line utility for controlling the CM17A firecracker module.

  Copyright (C) 1999, 2002 Matt Armstrong
  All rights reserved.

  Redistribution and use in source and binary forms, with or
  without modification, are permitted provided that the
  following conditions are met:

  1. Redistributions of source code must retain the above
     copyright notice, this list of conditions and the following
     disclaimer.

  2. Redistributions in binary form must reproduce the above
     copyright notice, this list of conditions and the following
     disclaimer in the documentation and/or other materials
     provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS
  IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
  OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
  OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
  OF SUCH DAMAGE.

=== Additional Licence for cm17a.c

In addition to the FlipIt project, the windows compatibility code was
derived from Adam Brigg's CM17A windows project.

  This code is (c)2000, Adam Briggs. All Rights Reserved.

  Permission to use, copy, and distribute this software and its
  documentation for any purpose and without fee is hereby granted,
  provided that the above copyright notice appear in all copies
  and that both that copyright notice and this permission notice
  appear in supporting documentation, and that the name Adam
  Briggs not be used in advertising or publicity pertaining to
  distribution of the software without specific, written prior
  permission.

                              *** DISCLAIMER ***

  ADAM BRIGGS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS
  SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
  AND FITNESS, IN NO EVENT SHALL ADAM BRIGGS BE LIABLE FOR ANY
  SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
  IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTUOUS ACTION,
  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
  THIS SOFTWARE.

== More X10 and CM17A Information

* http://x10.com
* http://www.pragmaticautomation.com/cgi-bin/pragauto.cgi/Monitor/Devices/BubbleBubbleBuildsInTrouble.rdoc
