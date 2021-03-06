#!/bin/sh

echo Let\'s do update EFL Packages

svn update

for e in eina eet evas ecore eeze efreet embryo edje e_dbus e ethumb elementary expedite
do 
	if [ -e $e ] && [ -d $e ]
	then 
		echo Start $e 
		cd $e
		./autogen.sh
		sudo make install -Wall -Wextra -Wshadow && sudo ldconfig
		cd ..
		echo End Done
	fi 
done

exit 0
