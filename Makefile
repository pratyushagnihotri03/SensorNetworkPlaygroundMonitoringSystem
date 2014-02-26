ifndef TARGET
TARGET=xm1000
endif

CONTIKI_PROJECT = main
all: $(CONTIKI_PROJECT)

CONTIKI_SOURCEFILES += vh400.c
CONTIKI_SOURCEFILES += ds1000-sensor.c

#CONTIKI = your-path-to-contiki
ifndef CONTIKI
  CONTIKI = /home/user/contiki-2.6/
endif

include $(CONTIKI)/Makefile.include
