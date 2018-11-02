FrontDoor
=========

This is a little program I hacked together to display information about the VMs I have floating about the place.
Often I need to ssh into to the machines to do things and, if I don't know the IP address due to DHCP lease changes I have to use the VM console.  This can be a pain.
I figured it would be nice to be able to see at a glance what the IP address of a VM was then I could just ssh in directly.

Requirements
------------

FrontDoor uses ncurses, so to compile it install the `ncurses-dev` package.

Installation on Ubuntu 16.04
----------------------------

The code currently only supports Linux.

1. Compile the code by running `make`
2. Move the program into place, I suggest `/usr/local/sbin`
3. Execute `systemctl edit getty@tty1` and add the following content

```
[Service]
ExecStart=
ExecStart=-/sbin/agetty --noclear -n -l /usr/local/sbin/frontdoor -8 %I $TERM
```

4. Reboot

Installation on older machines
------------------------------
Alter `/etc/init/tty1.conf` as follows

```
# tty1 - getty
#
# This service maintains a getty on tty1 from the point the system is
# started until it is shut down again.

start on stopped rc RUNLEVEL=[2345]
stop on runlevel [!2345]

respawn
exec /sbin/getty -n -l /usr/local/sbin/frontdoor -8 38400 tty1
```

After a reboot you should see the new front door screen looking something like the following:

![Example Output](https://github.com/dparnell/frontdoor/raw/master/images/frontdoor.png)
