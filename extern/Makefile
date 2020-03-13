EXTERN_DIR=${shell pwd}

default:
	tar -xzf TACS_extern.tar.gz
	tar -xzf metis*.tar.gz
	tar -xzf UFconfig*.tar.gz
	tar -xzf AMD*.tar.gz
	unzip tecio2009.zip

	echo; (cd UFconfig && make CFLAGS="-O3 -fPIC -fexceptions") || exit 1;
	echo; (cd AMD && make CFLAGS="-O3 -fPIC -fexceptions") || exit 1;
	echo; (cd metis-5.1.0 && make config prefix=${EXTERN_DIR}/metis/ CFLAGS="-fPIC") || exit 1;
	echo; (cd metis-5.1.0 && make install) || exit 1;
	cd tecio && sh Runmake linuxi64.24 || exit 1;