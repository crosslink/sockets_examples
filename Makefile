SRCDIR = src
OBJDIR = obj
BINDIR = bin

CC = gcc

#SOURCES  := $(wildcard $(SRCDIR)/*.c)

programs=socket_select mcastserver mcastclient ip_detect get_ips get_ips_v6

all: $(programs)

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

socket_select: $(OBJDIR)/socket_select.o
	$(CC) -o $@ $^ $(LDFLAGS) 
	
mcastserver: $(OBJDIR)/mcastserver.o
	$(CC) -o $@ $^ $(LDFLAGS)
	 
mcastclient: $(OBJDIR)/mcastclient.o
	$(CC) -o $@ $^ $(LDFLAGS)
	 
ip_detect: $(OBJDIR)/ip_detect.o
	$(CC) -o $@ $^ $(LDFLAGS) 
	
get_ips: $(OBJDIR)/get_ips.o
	$(CC) -o $@ $^ $(LDFLAGS) 
	
get_ips_v6: $(OBJDIR)/get_ips_v6.o
	$(CC) -o $@ $^ $(LDFLAGS) 
	
clean:
	rm -rf obj/*
	rm -rf bin/*
	
	
	