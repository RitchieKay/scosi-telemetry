DYNLIBS = libsi_proc.so
OBJECTS = si_proc_parameter.o si_proc_utilities.o si_proc_packet.o si_proc_pvt.o
LDFLAGS = -L.
INSTALL_BIN = ../installation/bin
INSTALL_LIB = ../installation/lib
INCFLAGS = -I../
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
