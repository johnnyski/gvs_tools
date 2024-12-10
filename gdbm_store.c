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
  fprintf(stderr, "Usage: %s [-f gdbmfile] key_str content_str\n", cmd);
  exit(-1);
}


void process_args(int argc, char **argv,
				  char **dbfile, char **key_str, char **content_str)
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

/* must have 2 strings listed */
  if (argc - optind != 2) usage(argv[0]);

  *key_str     = (char *) strdup(argv[optind]);
  *content_str = (char *) strdup(argv[optind+1]);
}

int main(int ac, char **av)
{
  char *dbfile;
  char *key_str, *content_str;

  GDBM_FILE gf;
  datum key, content;

  dbfile = "/tmp/monitor.db"; /* Default. */

  process_args(ac, av, &dbfile, &key_str, &content_str);

  gf = gdbm_open(dbfile, 512, GDBM_WRCREAT, 0664, 0);
  if(gf==NULL) {
	fprintf(stderr,"ERROR: %s\n", dbfile);
	perror(dbfile);
	exit(0);
  }

  key.dptr = key_str;
  key.dsize = strlen(key_str) + 1;

  content.dptr = content_str;
  content.dsize = strlen(content_str) + 1;

  if (gdbm_store(gf, key, content, GDBM_REPLACE) != 0) {
	fprintf(stderr, "%s: %s in %s\n", key.dptr, gdbm_strerror(gdbm_errno),
			dbfile);
  }

  gdbm_close(gf);

  exit (0);
}
