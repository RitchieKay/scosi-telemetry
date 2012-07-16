#!/usr/bin/python

import curses
from curses.wrapper import *
import sys
import os
import time
import datetime
import scosi_value
from si_sfs_parameter import *

DISPLAY_DESCRIPTION_Y_POS = 1
DISPLAY_DESCRIPTION_NAME_POS = 1
DISPLAY_DESCRIPTION_DESCR_POS = 1
Y_PARAM_MARGIN = 3
PARAM_NAME_POS = 2
PARAM_NAME_WIDTH = 8
PARAM_DESCRIPTION_POS = 12
PARAM_DESCRIPTION_WIDTH = 24
PARAM_VALUE_POS = 38
PARAM_VALUE_WIDTH = 16
PARAM_UNIT_POS = 56
PARAM_UNIT_WIDTH = 8
PARAM_VALID_POS = 66
PARAM_VALID_WIDTH = 8
PARAM_TIME_POS = 76
PARAM_TIME_WIDTH = 26

DEFAULT_COLOR = 1
INVALID_COLOR = 2
WARN_COLOR = 3
ALARM_COLOR = 4

def asdtime(t):

    d = datetime.datetime.fromtimestamp(t)
    time_str = d.strftime('%Y.%j.%H.%M.%S.')
    milli_str = (str(t - int(t)) + '000')[2:5] 

    return time_str + milli_str

def setup_display(stdscr, and_params, and_details):

    stdscr.clear()
    stdscr.border(0)
    (maxy,maxx) = stdscr.getmaxyx()

    stdscr.addstr(DISPLAY_DESCRIPTION_Y_POS, DISPLAY_DESCRIPTION_NAME_POS, and_details['name'])
    stdscr.addstr(DISPLAY_DESCRIPTION_Y_POS, DISPLAY_DESCRIPTION_DESCR_POS, and_details['description'])

    for location in and_params:
        info = and_params[location]['info']
        value = and_params[location]['value']

        if (Y_PARAM_MARGIN + location) >= maxy - Y_PARAM_MARGIN: continue

        stdscr.addstr(Y_PARAM_MARGIN + location, PARAM_NAME_POS, \
                      info['name'][0:PARAM_NAME_WIDTH], curses.color_pair(1))

        stdscr.addstr(Y_PARAM_MARGIN + location, PARAM_DESCRIPTION_POS, \
                      info['description'][0:PARAM_DESCRIPTION_WIDTH], curses.color_pair(1))

        stdscr.addstr(Y_PARAM_MARGIN + location, PARAM_UNIT_POS, \
                      info['unit'][0:PARAM_UNIT_WIDTH], curses.color_pair(1))


def refresh_display(stdscr, and_params):

    (maxy,maxx) = stdscr.getmaxyx()
    for location in and_params:
        info = and_params[location]['info']
        value = and_params[location]['value']
        value_str = ' '*(PARAM_VALUE_WIDTH-len(str(value.value()))) + str(value.value())


        if (Y_PARAM_MARGIN + location) >= maxy - Y_PARAM_MARGIN: continue
        
        if value.validity():

            stdscr.addstr(Y_PARAM_MARGIN + location, PARAM_VALID_POS, \
                          '  VALID', curses.color_pair(DEFAULT_COLOR,))
            stdscr.addstr(Y_PARAM_MARGIN + location, PARAM_TIME_POS, \
                          asdtime(value.time()), curses.color_pair(DEFAULT_COLOR))
            stdscr.addstr(Y_PARAM_MARGIN + location, PARAM_VALUE_POS, \
                          value_str, curses.color_pair(DEFAULT_COLOR,))

        else:
            stdscr.addstr(Y_PARAM_MARGIN + location, PARAM_VALID_POS, \
                          'INVALID', curses.color_pair(INVALID_COLOR,))
            stdscr.addstr(Y_PARAM_MARGIN + location, PARAM_TIME_POS, \
                          asdtime(value.time()), curses.color_pair(INVALID_COLOR))
            stdscr.addstr(Y_PARAM_MARGIN + location, PARAM_VALUE_POS, \
                          value_str, curses.color_pair(INVALID_COLOR,))

def myApp(stdscr, and_params, and_details):

    curses.init_pair(DEFAULT_COLOR, curses.COLOR_GREEN, curses.COLOR_BLACK)
    curses.init_pair(INVALID_COLOR, curses.COLOR_WHITE, curses.COLOR_BLACK)
    curses.init_pair(WARN_COLOR, curses.COLOR_GREEN, curses.COLOR_YELLOW)
    curses.init_pair(ALARM_COLOR, curses.COLOR_GREEN, curses.COLOR_RED)
    curses.cbreak()
    stdscr.leaveok(True)

   
    (maxy,maxx) = stdscr.getmaxyx()

    setup_display(stdscr, and_params, and_details)
    stdscr.refresh()

    stdscr.nodelay(1)
    x = 0
    while(True):
      x = stdscr.getch()
      if x < 0:
          refresh_display(stdscr, and_params)
          time.sleep(1)
      else:
          setup_display(stdscr, and_params, and_details)
#        break


#curses.endwin()

# Have to open the pvt map
pvt_file = os.getenv('SCOSI_TABLE_DIR') + '/pvt.tlb'
pvt = open(pvt_file, 'r+b')
pvt_map = mmap.mmap(pvt.fileno(), os.stat(pvt.name).st_size)
scosi_value.pvt_map = pvt_map

if len(sys.argv) != 2:
    print 'Usage:', sys.argv[0], 'and file'
    sys.exit(1)

params = si_sfs_parameter()

and_ = open(sys.argv[1], 'r')

and_params = {}
and_details = {}
header = True
for line in and_:
   values = line.strip().split('\t')
   # header
   if header:
       (and_details['name'], and_details['description'], and_details['type']) = values
       header = False
   else:

       (position, parameter, form) = values
       and_params[int(position)] = {'value':params.get_parameter(parameter),
                                   'info':params.get_parameter_info(parameter)}

wrapper(myApp, and_params, and_details)
#while True:
#    print '+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++'
#    for param in and_params:
#        info = and_params[param]['info']
#        value = and_params[param]['value']
#
#        print info['name'],'\t', info['description'], '\t',value.value(), '\t',info['unit'], '\t', value.time()
#
#    time.sleep(2)
