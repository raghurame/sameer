#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

/*

ARGUMENTS TO PASS:
~~~~~~~~~~~~~~~~~

argv[0] = program
argv[1] = input dump file name
argv[2] = degree of polymerization
argv[3] = number of chains

*/

typedef struct chainEnds
{
	float x1, y1, z1, x2, y2, z2;
} CHAIN_ENDS;

int calculateNatoms (FILE *input)
{
	rewind (input);
	int nAtoms;
	char lineString[2000];

	for (int i = 0; i < 4; ++i) {
		fgets (lineString, 2000, input); }

	sscanf (lineString, "%d", &nAtoms);

	return nAtoms;
}

int calculateNtimeframes (FILE *input, int nAtoms)
{
	rewind (input);
	int nTimeframes, nLines = 0;
	char lineString[2000];

	while (fgets (lineString, 2000, input) != NULL)
	{
		nLines++;
	}

	nTimeframes = nLines / (nAtoms + 9);

	return nTimeframes;
}

CHAIN_ENDS *fetchChainEnds (CHAIN_ENDS *ends, FILE *input, int nAtoms, int degreeOfPolymerization, int nChains)
{
	char lineString[2000];
	int currentLine = 0, currentTimestep = 0;

	while (fgets (lineString, 2000, input) != NULL)
	{
		currentLine++;

		if (currentLine > 9 && currentLine <= 9 + nAtoms)
		{
			sscanf ()
		}
	}

	return ends;
}

int main(int argc, char const *argv[])
{
	FILE *input;
	input = fopen (argv[1], "r");

	int degreeOfPolymerization = atoi (argv[2]), nChains = atoi (argv[3]);

	int nAtoms = calculateNatoms (input), nTimeframes = calculateNtimeframes (input, nAtoms);

	CHAIN_ENDS *ends;
	ends = (CHAIN_ENDS *) malloc (nChains * sizeof (CHAIN_ENDS));

	ends = fetchChainEnds (ends, input, nAtoms, degreeOfPolymerization, nChains);

	fclose (input);
	return 0;
}