#!/usr/bin/env python

"""
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as 
published by the Free Software Foundation, either version 3 of the 
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
"""

import os

if os.name == 'nt':  # Windows
    os = 'nt'
    import msvcrt
else:                # Posix (Linux, OS X)
    os = 'LINUX'
    import sys
    import termios
    import atexit
    from select import select

# special key definitions
ENTER = 10
ESC = 27
BACKSPACE = 127
TAB = 9


TTY_LEFT = 68
TTY_RIGHT = 67
TTY_UP = 65
TTY_DOWN = 66

WIN_LEFT = 75
WIN_RIGHT = 77
WIN_UP = 72
WIN_DOWN = 80

class KBHit:
    """ this class does the work """    
    def __init__(self):
        """Creates a KBHit object to get keyboard input """

        if os == 'LINUX':
            # Save the terminal settings
            self.fd = sys.stdin.fileno()
            self.new_term = termios.tcgetattr(self.fd)
            self.old_term = termios.tcgetattr(self.fd)
    
            # New terminal setting unbuffered
            self.new_term[3] = (self.new_term[3] & ~termios.ICANON & ~termios.ECHO)
            termios.tcsetattr(self.fd, termios.TCSAFLUSH, self.new_term)
    
            # Support normal-terminal reset at exit
            atexit.register(self.set_normal_term)

    
    def set_normal_term(self):
        """ Resets to normal terminal.  On Windows does nothing """
        if os == 'LINUX':
            termios.tcsetattr(self.fd, termios.TCSAFLUSH, self.old_term)


    def getch(self):
        """ Returns a keyboard character after kbhit() has been called """
        if os == 'nt':
            return msvcrt.getch()
        else:
            ch = sys.stdin.read(1)
            if(ord(ch) == 27): 
                n1, n2 = ord(sys.stdin.read(1)), ord(sys.stdin.read(1))
                if( n1 == 91):
                    if( n2 == TTY_LEFT):
                        return chr(WIN_LEFT)
                    if( n2 == TTY_RIGHT):
                        return chr(WIN_RIGHT)
                    if( n2 == TTY_UP):
                        return chr(WIN_UP)
                    if( n2 == TTY_DOWN):
                        return chr(WIN_DOWN)
            return ch
                    
                    
            # return sys.stdin.read(1)

    def kbhit(self):
        """ Returns True if keyboard character was hit, False otherwise. """
        if os == 'nt':
            return msvcrt.kbhit()
        else:
            dr, dw, de = select([sys.stdin], [], [], 0)
            return dr != []
    
