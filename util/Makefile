DYNLIBS = libsi_util.so
OBJECTS = si_util_mmap.o si_util_packet.o si_util_values.o si_util_utilities.o si_util_parameter.o
LDFLAGS = -L.
INCFLAGS = -I../
INSTALL_BIN = ../installation/bin
INSTALL_LIB = ../installation/lib
CCFLAGS = -Wall
SHARED = -shared

CC = gcc

all : $(DYNLIBS)

$(DYNLIBS) : $(OBJECTS)
	@echo "Building dynamic library $@"
	@$(CC) $(SHARED) $(OBJECTS) -o $@ 

%.o : %.c %.h
	@echo "Compiling $@ to $<"
	@$(CC) -c $< -o $@ $(INCFLAGS) $(CCFLAGS)

clean :
	rm -rf $(OBJECTS) $(DYNLIBS) $(TARGET)

install :
	@mkdir -p $(INSTALL_BIN) $(INSTALL_LIB)
	@cp $(DYNLIBS) $(INSTALL_LIB)
#	@cp  $(TARGET) $(INSTALL_BIN) 
# DO NOT DELETE
