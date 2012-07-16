from struct import *
import time
import mmap

# Globals

pvt_map = 0

nonValueRecordLength = 16

class sfs_value(object):

    __location = -1
    __rawValue = 0
    __engValue = 0
    __bytelen = 0
    __extract_pattern = ''
    __time = 0.0
    __calibrationType = ''
    __validity = 0
    __limits = 0

    def __init__(self, bytelen):
        self.init(bytelen)
    
    def init(self, bytelen):
        if type(pvt_map).__name__ != 'mmap':
	    raise Exception('pvt_map not defined. Cannot use value objects')
        self.__rawValue = 0
	self.__bytelen = bytelen

    def setLocation(self, location, extract_pattern):
        if type(pvt_map).__name__ != 'mmap':
	    raise Exception('pvt_map not defined. Cannot use value objects')
        self.__location = location
	self.__extract_pattern = extract_pattern

    def setCalibrationType(self, type):
        if type == 'S' or type == 'N':
	    self.__calibrationType = type
        else:
	    type = ''

    def location(self):
        return self.__location

    def value(self):
        if len(self.__calibrationType) > 0:
	    return self.eng()
        else:
	    return self.raw()

    def raw(self):
        self.refresh_value()
        return self.__rawValue

    def eng(self):
        self.refresh_value()
        return self.__engValue

    def time(self):
        self.refresh_value()
        return self.__time

    def validity(self):
        self.refresh_value()
        return self.__validity == 0

    def validity_bits(self):
        self.refresh_value()
        return hex(self.__validity)

    def refresh_value(self):
        if self.__location >= 0:
            global pvt_map

            if self.__calibrationType == 'N':
                (tv_sec, tv_usec, self.__validity, self.__limits, self.__rawValue, self.__engValue) =  \
	                 unpack('IIII' + self.__extract_pattern + 'd', \
    	                 pvt_map[self.__location:self.__location + self.__bytelen + nonValueRecordLength + 8])
            elif self.__calibrationType == 'S':
                (tv_sec, tv_usec, self.__validity, self.__limits, self.__rawValue, self.__engValue) =  \
	                 unpack('IIII' + self.__extract_pattern + '16s', \
    	                 pvt_map[self.__location:self.__location + self.__bytelen + nonValueRecordLength + 16])

                self.__engValue = self.__engValue[0:self.__engValue.index('\x00')]
            else:
                (tv_sec, tv_usec, self.__validity, self.__limits, self.__rawValue) = \
		         unpack('IIII' + self.__extract_pattern, \
    	                 pvt_map[self.__location:self.__location + self.__bytelen + nonValueRecordLength])

            self.__time = tv_sec + tv_usec *1E-6
    # All these cause a refresh

    def __repr__(self):
        self.refresh_value()
        return str(self.value())

    def __add__(self, other):
        self.refresh_value()
        try:
	    return self.value() + other.value()
        except AttributeError:
	    return self.value() + other

    def __radd__(self, other):
        self.refresh_value()
	return self.value() + other 

    def __sub__(self, other):
        self.refresh_value()
        try:
            return self.value() - other.value()
        except AttributeError:
            return self.value() - other
  
    def __rsub__(self, other):
        self.refresh_value()
	return self.value() - other 
	    
    def __mul__(self, other):
        self.refresh_value()
        try:
            return self.value() * other.value()
        except AttributeError:
            return self.value() * other

    def __rmul__(self, other):
        self.refresh_value()
	return self.value() * other 

    def __div__(self, other):
        self.refresh_value()
        try:
            return self.value() / other.value()
        except AttributeError:
            return self.value() / other

    def __rdiv__(self, other):
        self.refresh_value()
	return other / self.value() 

    def __pow__(self, other):
        self.refresh_value()
        try:
            return self.value() ** other.value()
        except AttributeError:
            return self.value() ** other

    def __rpow__(self, other):
        self.refresh_value()
	return other ** self.value() 

    def __lshift__(self, other):
        self.refresh_value()
        try:
            return self.value() << other.value()
        except AttributeError:
            return self.value() << other

    def __rlshift__(self, other):
        self.refresh_value()
	return other << self.value() 

    def __rshift__(self, other):
        self.refresh_value()
        try:
            return self.value() >> other.value()
        except AttributeError:
            return self.value() >> other

    def __rrshift__(self, other):
        self.refresh_value()
	return other >> self.value() 

    def __mod__(self, other):
        self.refresh_value()
        try:
            return self.value() % other.value()
        except AttributeError:
            return self.value() % other

    def __rmod__(self, other):
        self.refresh_value()
	return other % self.value() 

    def __and__(self, other):
        self.refresh_value()
        try:
            return self.value() & other.value()
        except AttributeError:
            return self.value() & other

    def __rand__(self, other):
        self.refresh_value()
	return other & self.value() 

    def __or__(self, other):
        self.refresh_value()
        try:
            return self.value() | other.value()
        except AttributeError:
            return self.value() | other

    def __ror__(self, other):
        self.refresh_value()
	return other | self.value() 

    def __xor__(self, other):
        self.refresh_value()
        try:
            return self.value() ^ other.value()
        except AttributeError:
            return self.value() ^ other

    def __rxor__(self, other):
        self.refresh_value()
	return other ^ self.value() 

    def __cmp__(self, other):
        self.refresh_value()
        try:
            return cmp(self.value(), other.value())
        except AttributeError:
            return cmp(self.value(), other)

    def __neg__(self):
        self.refresh_value()
        return -self.value()

    def __abs__(self):
        self.refresh_value()
        return abs(self.value())

    def __hex__(self):
        self.refresh_value()
        return hex(self.value())

    def __oct__(self):
        self.refresh_value()
        return oct(self.value())

    def __str__(self):
        self.refresh_value()
        return str(self.value())
    def __int__(self):
        self.refresh_value()
        return int(self.value())
    def __float__(self):
        self.refresh_value()
        return float(self.value())

class valueInt(sfs_value):
    def __init__(self):
        sfs_value.init(self, 4)
    def setLocation(self, loc):
        sfs_value.setLocation(self, loc, 'i')
    def setValue(self, value, validity = 0):
        sfs_value.setValue(self, int(value), validity)

class valueUInt(sfs_value):
    def __init__(self):
        sfs_value.init(self, 4 )
    def setLocation(self, loc):
        sfs_value.setLocation(self, loc, 'I')
    def setValue(self, value, validity = 0):
        sfs_value.setValue(self,int(value), validity)

class valueFloat(sfs_value):
    def __init__(self):
        sfs_value.init(self, 4)
    def setLocation(self, loc):
        sfs_value.setLocation(self, loc, 'f')
    def setValue(self, value, validity = 0):
        sfs_value.setValue(self, float(value), validity)

class valueDouble(sfs_value):
    def __init__(self):
        sfs_value.init(self, 8)
    def setLocation(self, loc):
        sfs_value.setLocation(self, loc, 'd')
    def setValue(self, value, validity = 0):
        sfs_value.setValue(self, float(value), validity)


