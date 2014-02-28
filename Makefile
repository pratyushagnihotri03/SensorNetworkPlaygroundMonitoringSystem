ifndef TARGET
TARGET=xm1000
endif

CONTIKI_PROJECT = main
all: $(CONTIKI_PROJECT)

CONTIKI_SOURCEFILES += vh400.c ds1000-sensor.c moisture.c sink.c light.c co2.c

#CONTIKI = your-path-to-contiki
ifndef CONTIKI
  CONTIKI = /home/user/contiki-2.6/
endif

include $(CONTIKI)/Makefile.include
