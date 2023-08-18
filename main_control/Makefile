DEBUG=1
STATIC=0
CC=gcc
CXX=g++
ARCH ?= $(shell uname -m)

###########################################################
CPPFLAGS+=-fPIE -Wall
CPPFLAGS += -fstack-protector-all
ifeq ($(DEBUG), 1)
	CPPFLAGS += -g3 -ggdb3 -DDEBUG
else
	CPPFLAGS += -g3 -O3 -Os -DNDEBUG
endif

###########################################################
ROOTDIR = ../

###########################################################
INCPATH += -I${ROOTDIR}/
INCPATH += -I${ROOTDIR}/common/
INCPATH += -I${ROOTDIR}/common/ASFramework
# INCPATH += -I${ROOTDIR}/common_plugin/
INCPATH += -I${ROOTDIR}/main_control/
CPPFLAGS += $(INCPATH)

###########################################################
# LIBS += ${ROOTDIR}/common/dmidecode/libdmidecode.a

LIBS+= -Wl,--as-needed
LIBS+= -lrt -lpthread -ldl -lz
LIBS += -pie -z relro -z now
ifeq ($(STATIC), 1)
	LIBS += -static-libgcc
endif

###########################################################
COMMON_OBJS += ${ROOTDIR}/common_plugin/path_manager.o
COMMON_OBJS += ${ROOTDIR}/common_plugin/signal_helper.o


PCAP_OBJS += ${ROOTDIR}/main_control/main.o
PCAP_OBJS += ${ROOTDIR}/main_control/my_pcap.o

TARGET_OBJS += $(PCAP_OBJS) 

###########################################################
TARGET += PCAP 
###########################################################
.PHONY : all
all: $(TARGET)

PCAP: $(COMMON_OBJS) $(PCAP_OBJS) 
	$(CXX) $(CPPFLAGS) -o ${ROOTDIR}/main_control/PCAP $(COMMON_OBJS) $(PCAP_OBJS) $(LIBS)

.PHONY:clean
clean:
	rm -f $(COMMON_OBJS) $(PCAP_OBJS) 
