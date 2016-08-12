#
#   Module:  model
#   Author:  Song Fang
#   Date:    2005.9.6
#


#-------------------------------------------------
# for Red Hat 9 & Oracle 9.2
#

# Oracle include & library path
ORAINCS     = -I${ORACLE_HOME}/precomp/syshdr \
              -I${ORACLE_HOME}/precomp/public \
              -I${ORACLE_HOME}/rdbms/public \
              -I${ORACLE_HOME}/plsql/public \
              -I. \
              -DLINUX -D_GNU_SOURCE -D_LARGEFILE64_SOURCE=1 -D_LARGEFILE_SOURCE=1 \
              -DSLTS_ENABLE -DSLMXMX_ENABLE -D_REENTRANT -DNS_THREADS
ORALIBS     = -L${ORACLE_HOME}/lib -L${ORACLE_HOME}/rdbms/lib 

# Proc include path setting
PIPATH      = include="${ORACLE_HOME}/precomp/public" \
              include="${ORACLE_HOME}/precomp/syshdr" \
              include="${ORACLE_HOME}/rdbms/public" \
              include="${ORACLE_HOME}/plsql/public" \
              sys_include="/usr/include" \
              sys_include="/usr/include/linux" \
              sys_include="/usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include"

# C Compiler include & library path setting
CIPATH      = 
CLPATH      = 

CFLAGS      = ${CC_CFLAGS} ${CIPATH} ${CLPATH}
PFLAGS      = ${PIPATH} USERID=$(PFLAG_USERID) sqlcheck=SEMANTICS fips=NO \
              code=KR_C parse=FULL \
              close_on_commit=YES cpool=YES

CC          = ${CC_COMPILER} ${CFLAGS}
PC          = proc ${PFLAGS}
LB          = ar -r
CP          = cp
RM          = rm -f


#-------------------------------------------------

.SUFFIXES:
.SUFFIXES: .o .c
.SUFFIXES: .o .pc
.SUFFIXES: .obj .c
.SUFFIXES: .obj .pc
.SUFFIXES: .c .pc
.c.o:
	$(CC) -o $(<:.c=.o) -c $<
.pc.o:
	$(PC) $<
	$(CC) -c $(<:.pc=.c)
	$(RM) $(<:.pc=.c) *.lis
.c.obj:
	$(CC) -c $<
.pc.obj:
	$(PC) $<
	$(CC) -c $(<:.pc=.c)
.pc.c:
	$(PC) $<


#-------------------------------------------------

TARGET      = model_build

AIM_PATH    = $(DIST_AIM_PATH)

SHARE_OBJS  = 

NEW_OBJS    = stock.o order.o trade.o withdraw.o price.o \
			  category.o industry.o sort.o \
			  global_data.o inside_market.o \
			  set_map.o stock_index.o 


#-------------------------------------------------

all: build

$(TARGET): $(NEW_OBJS)
#	$(CC) -o $@ ${SHARE_OBJS} $(NEW_OBJS) test.c

build: $(TARGET)

clear: build
	@$(RM) $(NEW_OBJS)

setup: build
	@$(CP) $(NEW_OBJS) $(AIM_PATH)

clean:
	@$(RM) $(TARGET) $(NEW_OBJS) *.o *.lis


#-------------------------------------------------
