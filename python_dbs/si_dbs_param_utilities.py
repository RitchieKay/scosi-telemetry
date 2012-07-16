#***************************************************************************
#
# PROJECT  : SCOS-I TM 
#
# SUBSYSTEM: PYTHON_DBS
#
# FILENAME : si_dbs_param_utilities.py
#
# AUTHOR   : Ritchie Kay 
#
# DATE     : 20.06.2010
#
# COPYRIGHT: Ritchie Kay, 2010
#
#***************************************************************************
#
# PURPOSE: si_dbs_param_utilities provides the get_type_len method for determine
#          parameter details
#
#***************************************************************************/
#
#
def get_type_len(ptc, pfc):

    ptype = 'X'
    bytelen = 0
    bitlen = 0
    if ptc < 4:
        ptype = 'U'
    elif ptc == 4: ptype = 'I'
    elif ptc == 5 and pfc == 1 : ptype = 'F'
    elif ptc == 5 : ptype = 'D'
    elif ptc == 6: ptype = 'U'
    elif ptc == 7: ptype = 'O'
    elif ptc == 8: ptype = 'A'
    elif ptc == 9: ptype = 'T'
    elif ptc == 10: ptype = 'R'

    if ptc == 1:
        bytelen = 1
        bitlen = 1
    elif ptc == 2:
        bytelen = (1+pfc)/8
        bitlen = pfc
    elif ptc == 3 or ptc ==4:
        bytelen = 4
        if pfc <= 12: bitlen = 4 + pfc
        elif pfc == 13: bitlen = 24
        elif pfc == 14: bitlen = 32
    elif ptc == 5:
        if pfc == 1:
            bytelen = 4
            bitlen = 32
        else:
            bytelen = 8
            bitlen = 32
    elif ptc == 6:
        bytelen = (1+pfc)/8
        bitlen = pfc
    elif ptc == 7 or ptc == 8:
        bytelen = pfc
        bitlen = pfc*8

    return (ptype, bytelen, bitlen)
