#!/bin/bash

echo -e "Generating rc file\n"
echo "export GLADE_CATALOG_SEARCH_PATH=\$GLADE_CATALOG_SEARCH_PATH:$PWD/../build/packet/lib" > gladeconf.rc
echo "export GLADE_MODULE_SEARCH_PATH=\$GLADE_MODULE_SEARCH_PATH:$PWD/../build/packet/lib" >> gladeconf.rc
echo 'Please add'
echo "source $PWD/gladeconf.rc"
echo 'To your ~./bashrc or ~/.profile file and restart your system or log off and on again'
echo -e "\nDone"