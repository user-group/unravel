/* pcmount.c */
/* mount a floppy on /pcfs */
/* written by Joseph Poole */

static  char    sccsid[] = "@(#)pcmount.c	1.3  4/26/95";

#include <string.h>
#define PCFS
#include <sys/mount.h>
# include <errno.h>
# include <stdio.h>

/*
struct pc_args args = { "/dev/sf0"};
*/
struct pc_args args = { "/dev/fd0"};

main( argc, argv)
int argc;
char** argv;
{
	int	code;

  /*
  if( strcmp( argv[0], "pcmount") == 0)
    if( argc == 1)
      mount( "pcfs", "/pcfs", M_NEWTYPE, &args);
    else if( argc == 2 && (strcmp( argv[1], "-ro") == 0))
      mount( "pcfs", "/pcfs", M_NEWTYPE | M_RDONLY, &args);
    else
      puts("usage : pcmount [-ro]");
  */
  if( strcmp( argv[0], "pcmount") == 0){
	  errno = 0;
      code = mount( "pcfs", "/pcfs", M_NEWTYPE | M_RDONLY, &args);
	  if (code){
		fprintf (stderr,"error code %d return %d ",errno,code);
		perror("Floppy disk mount");
	  }
	  else printf ("Floppy disk mounted\n");
	}
  else {
    code = unmount( "/pcfs");
	  if (code){
		perror("Floppy disk unmount");
	  }
	  else printf ("Floppy disk unmounted\n");
	}
}

