#include <stdio.h>
#include <gdbm.h>
#include <malloc.h>
#include <string.h>

/*
 * Simply list the keys and content. 
 */

int main (int ac, char **av)
{
  char *infile = "/tmp/monitor.db";
  GDBM_FILE gf;
  datum key, content;
  char *key_str, *content_str;

  if (ac == 2) infile = av[1];
  else if (ac > 2) {
	fprintf(stderr, "Usage: %s gdbm_file\n", av[0]);
	exit(-1);
  }

  gf = gdbm_open(infile, 512, GDBM_READER, 0, 0);
  if (gf == NULL) {
	perror(gdbm_strerror(gdbm_errno));
	perror(infile);
	exit(-1);
  }
  for(key = gdbm_firstkey(gf); key.dptr; key = gdbm_nextkey(gf, key)) {

	key_str = (char *)calloc(key.dsize+1, sizeof(char));
	memmove(key_str, key.dptr, key.dsize);
	content = gdbm_fetch(gf, key);
	content_str = (char *)calloc(content.dsize+1, sizeof(char));
	memmove(content_str, content.dptr, content.dsize);
	
	printf("Key<<%s>>, Content<<%s>>\n", key_str, content_str);

	free(key_str); free(content_str);

  }

  gdbm_close(gf);
  exit(0);
}
