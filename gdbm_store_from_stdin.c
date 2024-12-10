#include <stdio.h>
#include <gdbm.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#ifdef __sgi
  #include <getopt.h>
#endif

void usage(char *cmd)
{
  fprintf(stderr, "Usage: %s [-f gdbmfile] key_str (reads data from stdin)\n", cmd);
  exit(-1);
}


void process_args(int argc, char **argv,
				  char **dbfile, char **key_str)
{
  extern char *optarg;
  extern int optind, opterr, optopt;

  int c;
  
  while ((c = getopt(argc, argv, "f:")) != -1)
        switch (c) {
        case 'f': *dbfile = (char *) strdup(optarg);  break;
        case '?': usage(argv[0]); break;
        default:  break;
        }

/* must have 1 strings listed */
  if (argc - optind != 1) usage(argv[0]);

  *key_str     = (char *) strdup(argv[optind]);
}

typedef struct _linked {
  char buf[10000];
  int n;
  struct _linked *next;
} Linked;

char *slurp_stdio(int *nbytes)
{
  Linked *head, *tail;
  char *bigbuf;
  int n = 0;
  head = tail = NULL;
  
  head = (Linked *)calloc(1, sizeof(Linked));
  tail = head;

  while ((tail->n = read(0, tail->buf, sizeof(tail->buf))) > 0) {
	tail->next = (Linked *)calloc(1, sizeof(Linked));
	n += tail->n;
	tail = tail->next;
  }

  bigbuf = (char *)calloc(n, sizeof(char));
  for(n = 0, tail = head; tail; tail = tail->next) {
	memcpy(&bigbuf[n], tail->buf, tail->n);
	n += tail->n;
  }
  *nbytes = n;
  return bigbuf;
}


int main(int ac, char **av)
{
  char *dbfile;
  char *key_str, *content_str;
  
  GDBM_FILE gf;
  datum key, content;
  int nbytes;

  dbfile = "/tmp/monitor.db"; /* Default. */

  process_args(ac, av, &dbfile, &key_str);

  gf = gdbm_open(dbfile, 512, GDBM_WRCREAT, 0664, 0);
  if(gf==NULL) {
	fprintf(stderr,"ERROR: %s\n", dbfile);
	perror(dbfile);
	exit(0);
  }

  key.dptr = key_str;
  key.dsize = strlen(key_str) + 1;

  /* Slurp the entire data (read from stdin) into memory. */
  /* Since we don't know how much we're reading.  Use a dynamically
   * allocated linked list to hold all the buffers.  Keep track of
   * the total bytes read.
   */
  content_str = slurp_stdio(&nbytes);
  content.dptr = content_str;
  content.dsize = nbytes;
  
  if (gdbm_store(gf, key, content, GDBM_REPLACE) != 0) {
	fprintf(stderr, "%s: %s in %s\n", key.dptr, gdbm_strerror(gdbm_errno),
			dbfile);
  }

  gdbm_close(gf);

  exit (0);
}
