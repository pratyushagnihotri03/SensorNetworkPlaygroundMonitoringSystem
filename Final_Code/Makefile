ifndef TARGET
TARGET=xm1000
endif

DEFINES=NETSTACK_MAC=csma_driver,\
 NETSTACK_RDC=xmac_driver,\
 RF_CHANNEL=26

CONTIKI_PROJECT = main
all: $(CONTIKI_PROJECT)

CONTIKI_SOURCEFILES += vh400.c ds1000-sensor.c moisture.c light.c co2.c

#CONTIKI = your-path-to-contiki
ifndef CONTIKI
  CONTIKI = /home/user/contiki-2.6/
endif

include $(CONTIKI)/Makefile.include
