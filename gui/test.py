#!/usr/bin/env python

from os import system
import curses
from curses.wrapper import *


def myApp(stdscr):

    curses.init_pair(1, curses.COLOR_GREEN, curses.COLOR_RED)


    stdscr.clear()
    stdscr.border(0)
    
#    (y,x) = stdscr.getmaxyx()
#
#    entrywin = stdscr.subwin(30, 30, 20, 20)
#    entrywin.border(0)
#    entrywin.addstr(10,2, ": ")
#    entrywin.refresh()
#
    stdscr.addstr(2, 2, "Please enter a number...", curses.color_pair(1))
    stdscr.refresh()

    x = stdscr.getch()

#curses.endwin()

wrapper(myApp)
