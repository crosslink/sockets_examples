SRCDIR = src
OBJDIR = obj
BINDIR = .

CC = gcc

SOURCES  := $(wildcard $(SRCDIR)/*.c)

all: socket_select mcastserver mcastclient ip_detect get_ips get_ips_v6

socket_select: $(SRCDIR)/socket_select.o
	$(CC) -o $@ $^ $(LDFLAGS) 
	
mcastserver: $(SRCDIR)/mcastserver.o
	$(CC) -o $@ $^ $(LDFLAGS)
	 
mcastclient: $(SRCDIR)/mcastclient.o
	$(CC) -o $@ $^ $(LDFLAGS)
	 
ip_detect: $(SRCDIR)/ip_detect.o
	$(CC) -o $@ $^ $(LDFLAGS) 
	
get_ips: $(SRCDIR)/get_ips.o
	$(CC) -o $@ $^ $(LDFLAGS) 
	
get_ips_v6: $(SRCDIR)/get_ips_v6.o
	$(CC) -o $@ $^ $(LDFLAGS) 
	