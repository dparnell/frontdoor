FrontDoor
=========

This is a little program I hacked together to display information about the VMs I have floating about the place.
Often I need to ssh into to the machines to do things and, if I don't know the IP address due to DHCP lease changes I have to use the VM console.  This can be a pain.
I figured it would be nice to be able to see at a glance what the IP address of a VM was then I could just ssh in directly.

Requirements
------------

FrontDoor uses ncurses, so to compile it install the `ncurses-dev` package.

Installation
------------

The code currently on supports Linux and has only been tested on Ubuntu 10.04 LTS.

1. Compile the code by running `make`
2. Move the program into place, I suggest `/usr/local/sbin`
3. Alter `/etc/init/tty1.conf as follows

<pre><code>
# tty1 - getty
#
# This service maintains a getty on tty1 from the point the system is
# started until it is shut down again.

start on stopped rc RUNLEVEL=[2345]
stop on runlevel [!2345]

respawn
exec /sbin/getty -n -l /usr/local/sbin/frontdoor -8 38400 tty1
</code></pre>

After a reboot you should see the new front door screen looking something like the following:

![Example Output](https://github.com/dparnell/frontdoor/raw/master/images/frontdoor.png)
