
TEMPLATE = app
TARGET = cppunit-test
INCLUDEPATH += .

HEADERS += Money.h \
    MoneyTestCase.h \
    MSVCOutputter.h \
    GccOutputter.h
SOURCES += MoneyApp.cpp \
    MoneyTestCase.cpp \
    MSVCOutputter.cpp \
    GccOutputter.cpp

LIBS += -lcppunit
