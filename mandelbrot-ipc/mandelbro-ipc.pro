TEMPLATE = subdirs

SUBDIRS += \
	mandelbrot-app \
	mandelbrot-worker

OTHER_FILES += \
	sdk/message.h \
	sdk/jobrequest.h \
	sdk/jobresult.h \
	sdk/messageutils.h
