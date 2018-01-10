#!/bin/bash

echo -e "Generating rc file\n"
echo "export GLADE_CATALOG_SEARCH_PATH=\$GLADE_CATALOG_SEARCH_PATH:$PWD/../build" > gladelib.rc
echo "export GLADE_MODULE_SEARCH_PATH=\$GLADE_MODULE_SEARCH_PATH:$PWD/../build" >> gladelib.rc
echo 'Please add'
echo "source $PWD/gladelib.rc"
echo 'To your ~./bashrc file and restart your system or log off and on again'
echo -e "\nDone"