#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ioctl.h>
#if __MACH__ || __NetBSD__ || __OpenBSD__ || __FreeBSD__
#include <sys/sysctl.h>
#endif
/* Include sockio.h if needed */
#ifndef SIOCGIFCONF
#include <sys/sockio.h>
#endif
#include <netinet/in.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#if __MACH__
#include <net/if_dl.h>
#endif

#include <curses.h>
#include <time.h>

#include <errno.h>
#include <linux/unistd.h>       /* for _syscallX macros/related stuff */
#include <linux/kernel.h>       /* for struct sysinfo */

#include "font.h"


void render(char* data) {
  char buffer[1024];
  char* eol_pos;

  while(*data) {
    eol_pos = strchr(data, '\n');
    if(eol_pos) {
      bzero(buffer, sizeof(buffer));
      strncpy(buffer, data, eol_pos - data + 1);

      addstr(buffer);
      clrtoeol();
      data = eol_pos + 1;
    } else {
      addstr(data);
      break;
    }
  }
}

// See https://github.com/ajrisi/lsif for the original lsif source code

/* On platforms that have variable length 
   ifreq use the old fixed length interface instead */
#ifdef OSIOCGIFCONF
#undef SIOCGIFCONF
#define SIOCGIFCONF OSIOCGIFCONF
#undef SIOCGIFADDR
#define SIOCGIFADDR OSIOCGIFADDR
#undef SIOCGIFBRDADDR
#define SIOCGIFBRDADDR OSIOCGIFBRDADDR
#endif

#define fatal_perror(X) do { perror(X), exit(1); } while(0)
 
/* 
 * this is straight from beej's network tutorial. It is a nice wrapper
 * for inet_ntop and helpes to make the program IPv6 ready
 */
char *get_ip_str(const struct sockaddr *sa, char *s, size_t maxlen)
{
  switch(sa->sa_family) {
  case AF_INET:
    inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr),
	      s, maxlen);
    break;
 
  case AF_INET6:
    inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr),
	      s, maxlen);
    break;
 
  default:
    strncpy(s, "Unknown AF", maxlen);
    return NULL;
  }
 
  return s;
}

int lsif(void)
{
  char            buf[8192] = {0};
  struct ifconf   ifc = {0};
  struct ifreq   *ifr = NULL;
  int             sck = 0;
  int             nInterfaces = 0;
  int             i = 0;
  char            ip[INET6_ADDRSTRLEN] = {0};
  struct ifreq    *item;
  struct sockaddr *addr;
  socklen_t       salen;
  char            hostname[NI_MAXHOST];
  char            output_buffer[1024];

  /* Get a socket handle. */
  sck = socket(PF_INET, SOCK_DGRAM, 0);
  if(sck < 0) {
    fatal_perror("socket");
    return 1;
  }
 
  /* Query available interfaces. */
  ifc.ifc_len = sizeof(buf);
  ifc.ifc_buf = buf;
  if(ioctl(sck, SIOCGIFCONF, &ifc) < 0) {
    fatal_perror("ioctl(SIOCGIFCONF)");
    return 1;
  }
  /* Iterate through the list of interfaces. */
  ifr = ifc.ifc_req;
  nInterfaces = ifc.ifc_len / sizeof(struct ifreq); 
  for(i = 0; i < nInterfaces; i++) {
    bzero(hostname, NI_MAXHOST);
    item = &ifr[i];
    
    /* Show the device name and IP address */
    addr = &(item->ifr_addr);

    switch(addr->sa_family) {
    case AF_INET:
      salen = sizeof(struct sockaddr_in);
      break;
    case AF_INET6:
      salen = sizeof(struct sockaddr_in6);
      break;
    default:
      salen = 0;
    }
   
    /* the call to get the mac address changes what is stored in the
       item, meaning that we need to determine the hostname now */
    //    getnameinfo(addr, salen, hostname, sizeof(hostname), NULL, 0, NI_NAMEREQD);


    /* Get the address 
     * This may seem silly but it seems to be needed on some systems 
     */
    if(ioctl(sck, SIOCGIFADDR, item) < 0) {
      fatal_perror("ioctl(OSIOCGIFADDR)");
    }
    sprintf(output_buffer, "%s\t%s",
	    item->ifr_name,
	    get_ip_str(addr, ip, INET6_ADDRSTRLEN));
    addstr(output_buffer);

    /* Lots of different ways to get the ethernet address */
#ifdef SIOCGIFHWADDR
    /* Linux */
    /* Get the MAC address */
    if(ioctl(sck, SIOCGIFHWADDR, item) < 0) {
      fatal_perror("ioctl(SIOCGIFHWADDR)");
      return 1;
    }
    
    /* display result */
    
    sprintf(output_buffer, "\t%02x:%02x:%02x:%02x:%02x:%02x",
	    (unsigned char)item->ifr_hwaddr.sa_data[0],
	    (unsigned char)item->ifr_hwaddr.sa_data[1],
	    (unsigned char)item->ifr_hwaddr.sa_data[2],
	    (unsigned char)item->ifr_hwaddr.sa_data[3],
	    (unsigned char)item->ifr_hwaddr.sa_data[4],
	    (unsigned char)item->ifr_hwaddr.sa_data[5]);
    addstr(output_buffer);
    
#elif SIOCGENADDR
    /* Solaris and possibly all SysVR4 */
    /* Get the MAC address */
    if(ioctl(sck, SIOCGENADDR, item) < 0) {
      fatal_perror("ioctl(SIOCGENADDR)");
    }
    
    /* display result */
    sprintf(output_buffer, " %02x:%02x:%02x:%02x:%02x:%02x",
	    (unsigned char)item->ifr_enaddr[0],
	    (unsigned char)item->ifr_enaddr[1],
	    (unsigned char)item->ifr_enaddr[2],
	    (unsigned char)item->ifr_enaddr[3],
	    (unsigned char)item->ifr_enaddr[4],
	    (unsigned char)item->ifr_enaddr[5]);
    addstr(output_buffer);

#elif __MACH__ || __NetBSD__ || __OpenBSD__ || __FreeBSD__
    /* MacOS X and all modern BSD implementations (I hope) */
    int                mib[6] = {0};
    int                len = 0;
    char               *macbuf; 
    struct if_msghdr   *ifm;
    struct sockaddr_dl *sdl;
    unsigned char      ptr[];
      
    mib[0] = CTL_NET;
    mib[1] = AF_ROUTE;
    mib[2] = 0;
    mib[3] = AF_LINK;
    mib[4] = NET_RT_IFLIST;
    mib[5] = if_nametoindex(item->ifr_name);
    if(mib[5] == 0)
      continue;

    if(sysctl(mib, 6, NULL, (size_t*)&len, NULL, 0) != 0) {
      fatal_perror("sysctl");
    }

    macbuf = (char *) malloc(len);
    if(macbuf == NULL) {
      fprintf(stderr, "\nUnable to allocate necessary memory: %d\n", len);
      exit(1);
    }

    if(sysctl(mib, 6, macbuf, (size_t*)&len, NULL, 0) != 0) {
      fatal_perror("sysctl");
    }

    ifm = (struct if_msghdr *)macbuf;
    sdl = (struct sockaddr_dl *)(ifm + 1);
    ptr = (unsigned char *)LLADDR(sdl);

    sprintf(output_buffer, " %02x:%02x:%02x:%02x:%02x:%02x", 
	    ptr[0], ptr[1], ptr[2], 
	    ptr[3], ptr[4], ptr[5]);
    addstr(output_buffer);

    free(macbuf);
      
#else
#error OS Distribution Not Recognized
#endif

    sprintf(output_buffer, " %s", hostname);
    addstr(output_buffer);
    clrtoeol();
    addstr("\n");
  }
 
  return 0;
}

#define LOAD_DIVIDER (float)(1 << SI_LOAD_SHIFT)

int main(void) {
  WINDOW *wnd;
  char buffer[256];
  char k;
  time_t t;
  struct tm tm;
  struct sysinfo s_info;
  int days, hours, mins;

  wnd = initscr();
  cbreak();
  noecho();
  clear();
  refresh();
  nodelay(wnd, true);
  while(1) {
    move(0, 0);
 
    gethostname(&buffer[0], sizeof(buffer));
    attron(A_BOLD);
    print_string(&buffer[0], NULL, NULL, true);
    attroff(A_BOLD);

    render("\n");
    lsif();
    render("\n");

    t = time(NULL);
    tm = *localtime(&t);

    sprintf(buffer, "System Time: %04d-%02d-%02d %02d:%02d:%02d\n\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    render(buffer);

    if(sysinfo(&s_info) == 0) {     
      sprintf(buffer, "Load Average: %0.2f %0.2f %0.2f\n\n", s_info.loads[0]/LOAD_DIVIDER, s_info.loads[1]/LOAD_DIVIDER, s_info.loads[2]/LOAD_DIVIDER);
      render(buffer);

      // Uptime
      days = s_info.uptime / 86400;
      hours = (s_info.uptime / 3600) - (days * 24);
      mins = (s_info.uptime / 60) - (days * 1440) - (hours * 60);

      sprintf(buffer, "Uptime: %d days, %d hours, %d minutes, %ld seconds\n\n", days, hours, mins, s_info.uptime % 60);
      render(buffer);

      sprintf(buffer, "Ram:\t%llum\tFree:\t%llum\nShared:\t%llum\tBuf:\t%llum\nSwap:\t%llum\tFree:\t%llum\n\n", 
	      s_info.totalram *(unsigned long long)s_info.mem_unit / (1024*1024), 
	      s_info.freeram *(unsigned long long)s_info.mem_unit / (1024*1024),
	      s_info.sharedram *(unsigned long long)s_info.mem_unit / (1024*1024),
	      s_info.bufferram *(unsigned long long)s_info.mem_unit / (1024*1024),
	      s_info.totalswap *(unsigned long long)s_info.mem_unit / (1024*1024),
	      s_info.freeswap *(unsigned long long)s_info.mem_unit / (1024*1024)
	      );
      render(buffer);
      // addstr(buffer);

      // Number of processes currently running.
      sprintf(buffer, "Number of processes: %d\n", s_info.procs);
      render(buffer);
    }

    attron(A_BOLD);
    render("\nPress any key to login\n");
    attroff(A_BOLD);
    clrtobot();

    refresh();

    if(getch() != ERR) {
      break;
    }

    sleep(1);
  }

  endwin();

  execlp("login", "login", NULL);
}
