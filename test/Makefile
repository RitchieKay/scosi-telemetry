TARGETS = si_dump_textual_calibration si_dump_numerical_calibration si_dump_packets test
INCFLAGS = -I../ 
LDFLAGS = -L../util -L../proc -L../sfs
SHAREDLIBS = -lsi_util -lsi_proc
INSTALL_BIN = ../installation/bin
INSTALL_LIB = ../installation/lib
SHARED = -shared
CCFLAGS = -Wall
CC = gcc

all: $(TARGETS)

$(DYNLIBS) : $(OBJECTS)
	@echo "Building dynamic library $@"
	@$(CC) $(SHARED) $(OBJECTS) -o $@ 

test : test.o
	@echo "Building executable target $@ with $(CC) $< -o $@ $(SHAREDLIBS) $(LDFLAGS)"
	@$(CC) $< -o $@ $(SHAREDLIBS) $(LDFLAGS)

si_dump_packets : si_dump_packets.o
	@echo "Building executable target $@ with $(CC) $< -o $@ $(SHAREDLIBS) $(LDFLAGS)"
	@$(CC) $< -o $@ $(SHAREDLIBS) $(LDFLAGS)

si_dump_textual_calibration : si_dump_textual_calibration.o
	@echo "Building executable target $@ with $(CC) $< -o $@ $(SHAREDLIBS) $(LDFLAGS)"
	@$(CC) $< -o $@ $(SHAREDLIBS) $(LDFLAGS)

si_dump_numerical_calibration : si_dump_numerical_calibration.o
	@echo "Building executable target $@ with $(CC) $< -o $@ $(SHAREDLIBS) $(LDFLAGS)"
	@$(CC) $< -o $@ $(SHAREDLIBS) $(LDFLAGS)

%.o : %.c 
	@echo "Compiling $@ to $< with $(CC) -c $< -o $@ $(INCFLAGS)"
	@$(CC) -c $< -o $@ $(INCFLAGS) $(CCFLAGS)

clean :
	rm -rf *.o $(TARGETS)
install :
	@mkdir -p $(INSTALL_BIN) $(INSTALL_LIB)
#	@cp $(DYNLIBS) $(INSTALL_LIB)
	@cp  $(TARGET) $(INSTALL_BIN) 
