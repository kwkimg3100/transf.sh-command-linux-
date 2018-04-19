#! /bin/bash

if [ $# != 2 ] && [ $# != 4 ]
then
echo 'transf_error : transf.sh [option]... [IP] or [PORT]...'
echo '	Description'
echo '		-s : server socket'
echo '			Using : transf.sh [option] [PORT]'
echo '		-c : client socket'
echo '			Using : transf.sh [option] [IP] [PORT]'
exit 1
fi

if [ $1 = '-s' ]
then
/usr/bin/./socket_server $2
elif [ $1 = '-c' ]
then
/usr/bin/./socket_client $2 $3 $4
fi
