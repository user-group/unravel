#
# 		@(#)makefile	1.12  7/26/96
#
#SHELL = /usr/bin/sh
INSTALL_DIR = .

LDLIBS = -lXaw -lXmu -lXt -lXext -lX11 -lm
# If you need to link with the "dl" library use the
# line below for LDLIBS
#LDLIBS = -lXaw -lXmu -lXt -lXext -lX11 -lm -ldl

#LDFLAGS = -L$(WINHOME)/lib
CFLAGS = -g -ansi -pedantic -Wall -Wunused-variable -Wno-pointer-to-int-cast \
-Wno-int-conversion -Wno-incompatible-pointer-types #-I$(WINHOME)/include

CC = gcc
I_PROGS = unravel select analyzer helpu
S_PROGS = slice_driver auto-slice pss-driver call-tree
PROGS = $(I_PROGS) $(S_PROGS) parser summary tsummary map u slink\
	visit-filter visit-ctrl visit-post vsort vprep
#	pcmount pcunmount visit-filter visit-ctrl visit-post a5parser vsort
I_OBJS = u.o unravel.o select.o analyzer.o unravel.o helpu.o
S_OBJS = slice_driver.o auto-slice.o pss-driver.o call-tree.o
#A5_OBJS = a5scan.o parser.o err.o a5gram.o const.o\
#	mem_alloc.o sym_tab.o xpr.o stmt.o chain.o control.o
PARSE_OBJS = kscan.o parser.o err.o kgram.o const.o\
	mem_alloc.o sym_tab.o xpr.o stmt.o chain.o control.o
SLICE_OBJS = sets.o slice.o slice-load.o pss.o
all : $(PROGS)
path.out :
	pwd > path.out
config.h : path.out
#	echo -n "# define HOME \"" >config.h
	cat path.out |sed 's/^/# define HOME \"/' | sed -e 's/$$/\"/' >config.h
	echo "#define  CC \"gcc\"" >>config.h
	echo "/* fix.h is needed by all files that also include config.h */">>config.h
	echo "#include \"fix.h\""  >>config.h
	echo "/* caddr_t is not available, so replace with void * for now.*/" >>config.h
	echo "#define caddr_t void *" >>config.h
#	find_cc >>config.h
vprep : config.h visit-prep
	sed s=HOME=`grep HOME config.h | sed 's/^.*E "/"/' | tr -d \"`= <visit-prep >vprep
	chmod +x vprep
multislice.o u.o: multislicep.h multislice.h
$(I_OBJS): config.h fix.h
$(S_OBJS) $(SLICE_OBJS): sets.h slice.h
$(PARSE_OBJS) slink.o pss.o slice-load.o: lif.h ansi_parse.h
#pcunmount : pcmount
#	ln pcmount pcunmount
#a5parser : $(A5_OBJS)
#	$(CC) $(CFLAGS) $(A5_OBJS) -o a5parser
parser : $(PARSE_OBJS)
	$(CC) $(CFLAGS) $(PARSE_OBJS) -o parser
$(S_PROGS) : $(SLICE_OBJS)
	$(CC) $(CFLAGS) $@.o $(SLICE_OBJS) -o $@
$(I_PROGS) :
	$(CC) $(CFLAGS) $@.o $(LDFLAGS) $(LDLIBS) -o $@
u : u.o $(SLICE_OBJS) MultiSlice.o history.o
	$(CC) $(CFLAGS) u.o MultiSlice.o history.o $(SLICE_OBJS) $(LDFLAGS) $(LDLIBS) -o $@
unravel : unravel.o
select : select.o
analyzer : analyzer.o
helpu : helpu.o
slice_driver : slice_driver.o
auto-slice : auto-slice.o
pss-driver : pss-driver.o
call-tree : call-tree.o
install :
	install -d $(INSTALL_DIR)/bin
	install -d $(INSTALL_DIR)/man/man1
	install  unravel $(INSTALL_DIR)/bin/unravel
	install  unravel.1 $(INSTALL_DIR)/man/man1/unravel.1
HOW_TO_INSTALL : how.nr
	nroff -me how.nr | col >HOW_TO_INSTALL
HOW_TO.ps : how.nr
	ptroff -me -t <how.nr >HOW_TO.ps
clean :
	/bin/rm -f *.[VYKHUTSo] *.out *.LIF core *.LINK SYSTEM HISTORY*
	/bin/rm -f $(PROGS)
	/bin/rm -f examples/*[A-Z]
tar-disk : clean
	date > LAST_TAR_DATE
	(SRC_DIR=`pwd | sed -e "s=^.*/==` ;cd .. ;\
		tar cvf /dev/rfd0 $$SRC_DIR )
	eject
	make all
tar-file : clean
	date > LAST_TAR_DATE
	(SRC_DIR=`pwd | sed -e "s=^.*/==` ;cd .. ;\
		tar cvf unravel_2.2+.tar $$SRC_DIR )
