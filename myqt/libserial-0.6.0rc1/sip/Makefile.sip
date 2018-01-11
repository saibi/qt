TARGET = libserial.so
OFILES = siplibserialcmodule.o siplibserialstdvector1300.o siplibserialstdvector1900.o siplibserialstdvector1800.o siplibserialstdvector2400.o siplibserialSerialPortReadTimeout.o siplibserialSerialPortUnsupportedBaudRate.o siplibserialSerialPortAlreadyOpen.o siplibserialSerialPortOpenFailed.o siplibserialSerialPortNotOpen.o siplibserialSerialPort.o siplibserialstdstring.o siplibserialstdinvalid_argument.o siplibserialstdruntime_error.o siplibserialstdlogic_error.o siplibserialstdexception.o siplibserialstd.o
HFILES = sipAPIlibserial.h 

CC = gcc
CXX = g++
LINK = g++
CPPFLAGS = -DNDEBUG -I. -I../src -I/usr/include/python2.6
CFLAGS =  -O2 -g -fPIC -Wall -W -D_REENTRANT
CXXFLAGS =  -O2 -g -fPIC -Wall -W -D_REENTRANT
LFLAGS = -shared -Wl,--version-script=libserial.exp
LIBS = -L../src/.libs -lserial -lpthread
.SUFFIXES: .c .o .cpp .cc .cxx .C


.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

all: $(TARGET)

$(OFILES): $(HFILES)

$(TARGET): $(OFILES)
	@echo '{ global: initlibserial; local: *; };' > libserial.exp
	$(LINK) $(LFLAGS) -o $(TARGET) $(OFILES) $(LIBS)

install: $(TARGET)
	@test -d $(DESTDIR)/usr/lib/python2.6/dist-packages || mkdir -p $(DESTDIR)/usr/lib/python2.6/dist-packages
	cp -f $(TARGET) $(DESTDIR)/usr/lib/python2.6/dist-packages/$(TARGET)

clean:
	-rm -f $(TARGET)
	-rm -f siplibserialcmodule.o
	-rm -f siplibserialstdvector1300.o
	-rm -f siplibserialstdvector1900.o
	-rm -f siplibserialstdvector1800.o
	-rm -f siplibserialstdvector2400.o
	-rm -f siplibserialSerialPortReadTimeout.o
	-rm -f siplibserialSerialPortUnsupportedBaudRate.o
	-rm -f siplibserialSerialPortAlreadyOpen.o
	-rm -f siplibserialSerialPortOpenFailed.o
	-rm -f siplibserialSerialPortNotOpen.o
	-rm -f siplibserialSerialPort.o
	-rm -f siplibserialstdstring.o
	-rm -f siplibserialstdinvalid_argument.o
	-rm -f siplibserialstdruntime_error.o
	-rm -f siplibserialstdlogic_error.o
	-rm -f siplibserialstdexception.o
	-rm -f siplibserialstd.o
	-rm -f libserial.exp
