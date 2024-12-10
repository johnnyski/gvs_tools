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
  fprintf(stderr, "Usage: %s [-f gdbmfile] key_str\n", cmd);
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

/* must have 1 string listed */
  if (argc - optind != 1) usage(argv[0]);

  *key_str     = (char *) strdup(argv[optind]);
}

int main(int ac, char **av)
{
  char *dbfile;
  char *key_str;

  GDBM_FILE gf;
  datum key, content;
  int i;

  dbfile = "/tmp/monitor.db"; /* Default. */

  process_args(ac, av, &dbfile, &key_str);

  gf = gdbm_open(dbfile, 512, GDBM_READER, 0, 0);
  if(gf==NULL) {
	fprintf(stderr,"ERROR: %s\n", dbfile);
	perror(dbfile);
	exit(0);
  }

  key.dptr = key_str;
  key.dsize = strlen(key_str) + 1;

  content = gdbm_fetch(gf, key);
  if (content.dptr == NULL)
/*	fprintf(stderr, "%s: %s in %s\n", key.dptr, gdbm_strerror(gdbm_errno),
			dbfile) */;
  else {
	for (i=0;i<content.dsize;i++) printf("%c", content.dptr[i]);
  }

  gdbm_close(gf);

  exit (0);
}
