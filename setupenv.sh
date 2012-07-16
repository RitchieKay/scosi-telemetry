export SCOSI_HOME=/home/ritchie/developments/scosi
export SCOSI_TABLE_DIR=$SCOSI_HOME/tables
export ASCII_DIR=$SCOSI_HOME/ascii
export RUNTIME_DIR=$SCOSI_HOME/runtime
export DISPLAYS_DIR=$RUNTIME_DIR/displays
export DBGEN_DIR=$SCOSI_HOME/gen
export PYTHONSTARTUP=$RUNTIME_DIR/runtime_setup.py

for dir in $SCOSI_HOME/python*; do 
   if [ -d $dir ]; then
     export PYTHONPATH=$dir:$PYTHONPATH
   fi  
done

for dir in $SCOSI_HOME/*; do
   if [ -d $dir ]; then
     export LD_LIBRARY_PATH=$dir:$LD_LIBRARY_PATH
   fi
done
