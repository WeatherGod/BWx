CC = gcc
CFLAGS = -O2

VERSION = 0.5

######################################################################
MYINC_PATH = /usr/include/BUtils
MYLIB_PATH = /usr/lib
######################################################################


all : libBMetar.so libBMOS.so

install : $(MYINC_PATH)/BMetar.h $(MYINC_PATH)/WeatherType.h $(MYINC_PATH)/CloudType.h $(MYLIB_PATH)/libBMetar.so \
          $(MYINC_PATH)/BMOS.h $(MYLIB_PATH)/libBMOS.so

$(MYINC_PATH)/BMetar.h : include/BMetar.h
	cp -f include/BMetar.h $(MYINC_PATH)/BMetar.h

$(MYINC_PATH)/WeatherType.h : include/WeatherType.h
	cp -f include/WeatherType.h $(MYINC_PATH)/WeatherType.h

$(MYINC_PATH)/CloudType.h : include/CloudType.h
	cp -f include/CloudType.h $(MYINC_PATH)/CloudType.h

$(MYLIB_PATH)/libBMetar.so : lib/libBMetar.so
	cp -f lib/libBMetar.so $(MYLIB_PATH)/libBMetar.so

libBMetar.so : lib/libBMetar.so

lib/libBMetar.so : BMetar.o WeatherType.o CloudType.o
	$(CC) $(LDFLAGS) -shared BMetar.o WeatherType.o CloudType.o -o lib/libBMetar.so -l stdc++

BMetar.o : include/BMetar.h src/BMetar.C
	$(CC) $(CFLAGS) -fPIC -c src/BMetar.C -o BMetar.o -I include/

WeatherType.o : include/WeatherType.h src/WeatherType.C
	$(CC) $(CFLAGS) -fPIC -c src/WeatherType.C -o WeatherType.o -I include/

CloudType.o : include/CloudType.h src/CloudType.C
	$(CC) $(CFLAGS) -fPIC -c src/CloudType.C -o CloudType.o -I include/

$(MYLIB_PATH)/libBMOS.so : lib/libBMOS.so
	cp -f lib/libBMOS.so $(MYLIB_PATH)/libBMOS.so

$(MYINC_PATH)/BMOS.h : include/BMOS.h
	cp -f include/BMOS.h $(MYINC_PATH)/BMOS.h

libBMOS.so : BMOS.o
	$(CC) $(LDFLAGS) -shared BMOS.o -o lib/libBMOS.so -l stdc++

BMOS.o : include/BMOS.h src/BMOS.C
	$(CC) $(CFLAGS) -fPIC -c src/BMOS.C -o BMOS.o -I include/

clean :
	- rm -f BMetar.o
	- rm -f WeatherType.o
	- rm -f CloudType.o
	- rm -f lib/libBMetar.so
	- rm -f BMOS.o
	- rm -f lib/libBMOS.so
	- rm -f "BWx-$(VERSION).tar"
	- rm -f "BWx-$(VERSION).tar.gz"
	- rm -f "BWx-$(VERSION).tar.bz2"

uninstall : remove

remove :
	- rm -f $(MYLIB_PATH)/libBMetar.so
	- rm -f $(MYINC_PATH)/BMetar.h
	- rm -f $(MYINC_PATH)/WeatherTypes.h
	- rm -f $(MYINC_PATH)/CloudTypes.h
	- rm -f $(MYLIB_PATH)/libBMOS.so
	- rm -f $(MYINC_PATH)/BMOS.h

tar : BWx-$(VERSION).tar

tar.gz : BWx-$(VERSION).tar.gz

tar.bz2 : BWx-$(VERSION).tar.bz2

BWx-$(VERSION).tar.gz : BWx-$(VERSION).tar
	gzip -f "BWx-$(VERSION).tar"

BWx-$(VERSION).tar.bz2 : BWx-$(VERSION).tar
	bzip2 -f "BWx-$(VERSION).tar"

BWx-$(VERSION).tar : include/BMetar.h include/BMOS.h include/CloudType.h include/WeatherType.h \
                     src/BMetar.C src/BMOS.C src/CloudType.C src/WeatherType.C \
                     Makefile
	mkdir -p "BWx-$(VERSION)/include"
	mkdir -p "BWx-$(VERSION)/src"
	mkdir -p "BWx-$(VERSION)/lib"
	cp -f include/BMetar.h include/BMOS.h include/CloudType.h include/WeatherType.h "BWx-$(VERSION)/include"
	cp -f src/BMetar.C src/BMOS.C src/CloudType.C src/WeatherType.C "BWx-$(VERSION)/src"
	cp -f Makefile "BWx-$(VERSION)/"
	-rm -rf "BWx-$(VERSION).tar"
	tar -cf "BWx-$(VERSION).tar" "BWx-$(VERSION)/"
	rm -rf "BWx-$(VERSION)/"

