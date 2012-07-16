INSTALL_BIN = installation/bin
INSTALL_LIB = installation/lib
INSTALL = installation 
RUNTIME = installation/runtime
TABLES = installation/tables

S2K = s2k_dev/model-cpd-rep/src/CPD/test

all:
	@cd util ; make
	@cd proc ; make
	@cd test ; make

clean:
	@cd util ; make clean
	@cd proc ; make clean
	@cd test ; make clean
	@rm -rf $(INSTALL)
	@rm -rf $(S2K)

install:
	@echo "Preparing installation ..."
	@cd util ; make install
	@cd proc ; make install
	@cd test ; make install
	@cp -r gen $(INSTALL)
	@cp -r ascii $(INSTALL)
	@cp -r python_sfs $(INSTALL)
	@cp setupenv.sh $(INSTALL)
	@rm -rf $(RUNTIME) ; mkdir $(RUNTIME)
	@rm -rf $(TABLES) ; mkdir $(TABLES)
	@echo "Done"
s2k :
	@echo "Preparing s2k development files"
	@rm -rf $(S2K) ; mkdir -p $(S2K)
	@cp */*.so $(S2K)
	@cp */*.h $(S2K)
	@cp scos2kIf/* $(S2K)
	@echo "Done"
	
