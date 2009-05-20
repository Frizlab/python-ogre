#! /usr/bin/python

"""Script which restores the key-repeat functionality under Gnome.

When Ogre crashes, it does not restore the hold down to repeat key
functionality. This Python script helps get it back.
"""

import gconf
import time

gconf_key = '/desktop/gnome/peripherals/keyboard/delay'

def reset_key(key):
    print "Poking %s" % key

    client = gconf.client_get_default()
    # Get the initial value
    i = client.get_int(key)

    # Change the initial value slightly
    client.set_int(key, i-1)

    # Wait some time for the change to activate
    time.sleep(0.100)

    # Restore the old value
    client.set_int(key, i)

if __name__ == '__main__':
    reset_key(gconf_key)
