/*
 * Copyright(C) 2011-2016 Pedro H. Penna <pedrohenriquepenna@gmail.com>
 * 
 * This file is part of Nanvix.
 * 
 * Nanvix is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Nanvix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Nanvix. If not, see <http://www.gnu.org/licenses/>.
 */

#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/* Software versioning. */
#define VERSION_MAJOR 1 /* Major version. */
#define VERSION_MINOR 0 /* Minor version. */

/* Program flags. */
#define LS_ALL   001     /* Print entries starting with dot? */
#define LS_INODE 002     /* Print inode numbers.             */
static int ls_flags = 0; /* Flags.                           */

/* Name of the directory to list. */
static char *dirname = NULL;



#include <unistd.h>

/* Software versioning. */
#define VERSION_MAJOR 1 /* Major version. */
#define VERSION_MINOR 0 /* Minor version. */

/*
 * Program arguments.
 */
static struct
{
	char *filename; /* File. */
	int flag;
} args = { NULL, 0 };

/*
 * Prints program version and exits.
 */

int rm(const char *pathname)
{
        DIR *dirp;                   /* Directory.               */
        struct dirent *dp;           /* Working directory entry. */
        char filename[NAME_MAX + 1]; /* Working file name.       */

        /* Open directory. */
        if ((dirp = opendir(pathname)) == NULL)
        {
                fprintf(stderr, "ls: cannot open %s\n", pathname);
                return (errno);
        }

        errno = 0;

        /* Read directory entries. */
        filename[NAME_MAX] = '\0';
        while ((dp = readdir(dirp)) != NULL)
        {
                strncpy(filename, dp->d_name, NAME_MAX);

                /* Suppress entries starting with dot. */
                if ((filename[0] == '.') && !(ls_flags & LS_ALL)){
                        continue;
		}

                /* Print inode number. */
                if (ls_flags & LS_INODE){
                        printf("%d ", (int)dp->d_ino);
		}

		char * path = (char*) malloc(1);
		strcpy(path, pathname);
		char * to_delete = strcat(path, filename);
		unlink(to_delete);
        }
        closedir(dirp);
        
        /* Error while reading. */
        if (errno != 0)
        {
                fprintf(stderr, "ls: cannot read %s\n", pathname);
                return (errno);
        }
        
        return (EXIT_SUCCESS);
}


static void version(void)
{
	printf("rm (Nanvix Coreutils) %d.%d\n\n", VERSION_MAJOR, VERSION_MINOR);
	printf("Copyright(C) 2011-2014 Pedro H. Penna\n");
	printf("This is free software under the "); 
	printf("GNU General Public License Version 3.\n");
	printf("There is NO WARRANTY, to the extent permitted by law.\n\n");
	
	exit(EXIT_SUCCESS);
}

/*
 * Prints program usage and exits.
 */
static void usage(void)
{
	printf("Usage: rm [options] <file>\n\n");
	printf("Brief: Removes files.\n\n");
	printf("Options:\n");
	printf("  --help    Display this information and exit\n");
	printf("  --version Display program version and exit\n");
	
	exit(EXIT_SUCCESS);
}

/*
 * Gets program arguments.
 */
static void getargs(int argc, char *const argv[])
{
	int i;     /* Loop index.       */
	char *arg; /* Current argument. */
		
	/* Read command line arguments. */
	for (i = 1; i < argc; i++)
	{
		arg = argv[i];
		
		/* Parse command line argument. */
		if (!strcmp(arg, "--help")) {
			usage();
		}
		else if (!strcmp(arg, "--version")) {
			version();
		}

		else if (!strcmp(arg, "-r")){
			args.flag = 1;
		}
		else {
			
			args.filename = arg;
			
		}
	}
	
	/* Missing argument. */
	if ((args.filename == NULL))
	{
		fprintf(stderr, "rm: missing argument\n");
		usage();
	}
}

/*
 * Creates a link between two files
 */
int main(int argc, char *const argv[])
{
	struct stat st;

	getargs(argc, argv);
	

	/* Failed to unlink(). */
	if (unlink(args.filename) < 0)
	{
		stat(args.filename, &st);
		if(S_ISDIR(st.st_mode) && args.flag == 1){
			
			rm(args.filename);
			return (EXIT_SUCCESS);

		}

		fprintf(stderr, "rm: cannot unlink()\n");
		return (EXIT_FAILURE);
	}
	
	return (EXIT_SUCCESS);
}
