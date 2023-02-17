#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

/*
	USAGE:
	~~~~~~

		argv[0] = ./program
		argv[1] = input filename with commented lines

		Output will be a new file without any comment lines.

	Example:
	~~~~~~~~

		gcc -o removeComments removeComments.c
		./removeComments inst_visc_eq_old.dat
*/

int main(int argc, char const *argv[])
{
	FILE *inputfile, *outputfile;
	inputfile = fopen (argv[1], "r");

	char *outputfilename;
	outputfilename = (char *) malloc (200 * sizeof (char));

	snprintf (outputfilename, 200, "%s.fixed", argv[1]);
	outputfile = fopen (outputfilename, "w");

	char lineString[2000];

	while (fgets (lineString, 2000, inputfile) != NULL)
	{
		if (lineString[0] != '#')
		{
			fprintf(outputfile, "%s", lineString);
		}
	}

	fclose (inputfile);
	fclose (outputfile);
	return 0;
}