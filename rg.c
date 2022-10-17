#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// This code calcukates the radius of gyration for each polymer chain and returns the ensemble averaged value

/*
ARGUMENTS TO PASS:
~~~~~~~~~~~~~~~~~
argv[0] = program
argv[1] = input dump file name
argv[2] = degree of polymerization (number of beads per chain)
argv[3] = number of chains
*/


//Before the main program function, we define all our functions and user defined data structures to be used.

typedef struct centerOfMass
{
	float x, y, z;
} CENTER_OF_MASS;

typedef struct coordinates
{
	float x, y, z;
} COORDINATES;

typedef struct radiusofgyration
{
	float x, y, z;
} RG;


int calculateNatoms (FILE *input) //Obviously, calculates the number of atoms in the dump file.
{
	rewind (input);
	int nAtoms;
	char lineString[2000];

	for (int i = 0; i < 4; ++i) //The atom information is on the 4th line in the dump file. 
     {
		fgets (lineString, 2000, input); //The fgets comes to the 4th line and stops.
     } 

	sscanf (lineString, "%d", &nAtoms); //sscanf scans the line string where fgets stopped, and returns the string there as integer.

	return nAtoms;
}

int calculateNtimeframes (FILE *input, int nAtoms) //Finds the total timeframes in the dump file. Takes the input file and nAtoms as input
{
	rewind (input); //Sets the file position to the starting line of the file.
	int nTimeframes, nLines = 0; //Initializing the variables
	char lineString[2000]; // Choose random high number. The value of '2000' must be larger than the longest line in dump file.

	while (fgets (lineString, 2000, input) != NULL) //Reads a text/line from the file 'input' and stores it in the variable lineString.
	{
		nLines++;
	}

	nTimeframes = nLines / (nAtoms + 9);

	return nTimeframes;
}

CENTER_OF_MASS **computeCenterOfMass (CENTER_OF_MASS **chainCOMs, FILE *input, int nAtoms, int degreeOfPolymerization, int nChains)
{
	rewind (input);

	char lineString[2000];
	int currentLine = 0, currentTimestep = 0, currentArrayCounter = 0;

	COORDINATES *chainCoordinates;
	chainCoordinates = (COORDINATES *) malloc (nAtoms * sizeof (COORDINATES));

	while (fgets (lineString, 2000, input) != NULL)
	{
		currentLine++;

		if (currentLine > 9 && currentLine <= 9 + nAtoms)
		{
			sscanf (lineString, "%*d %*d %*f %*f %*f %f %f %f\n", &chainCoordinates[currentLine - 10].x, &chainCoordinates[currentLine - 10].y, &chainCoordinates[currentLine - 10].z);
		}

		if (currentLine == (nAtoms + 10))
		{
			currentArrayCounter = 0;

			for (int j = 0; j < nChains; ++j)
			{
				chainCOMs[currentTimestep][j].x = 0;
				chainCOMs[currentTimestep][j].y = 0;
				chainCOMs[currentTimestep][j].z = 0;
			}

			for (int j = 0; j < nChains; ++j)
			{
				for (int k = 0; k < degreeOfPolymerization; ++k)
				{
					chainCOMs[currentTimestep][j].x += chainCoordinates[currentArrayCounter].x;
					chainCOMs[currentTimestep][j].y += chainCoordinates[currentArrayCounter].y;
					chainCOMs[currentTimestep][j].z += chainCoordinates[currentArrayCounter].z;

					currentArrayCounter++;
				}

				chainCOMs[currentTimestep][j].x /= degreeOfPolymerization;
				chainCOMs[currentTimestep][j].y /= degreeOfPolymerization;
				chainCOMs[currentTimestep][j].z /= degreeOfPolymerization;
			}

			currentLine = 0;
			currentTimestep++;
		}
	}

	return chainCOMs;
}

RG **computeRadiusofGyration (RG **chainRGs, CENTER_OF_MASS **chainCOMs, int nAtoms, int degreeOfPolymerization, int nChains)
{
	rewind (input);

	char lineString[2000];
	int currentLine = 0, currentTimestep = 0, currentArrayCounter = 0;

	COORDINATES *chainCoordinates;
	chainCoordinates = (COORDINATES *) malloc (nAtoms * sizeof (COORDINATES));

	while (fgets (lineString, 2000, input) != NULL)
	{
		currentLine++;

		if (currentLine > 9 && currentLine <= 9 + nAtoms)
		{
			sscanf (lineString, "%*d %*d %*f %*f %*f %f %f %f\n", &chainCoordinates[currentLine - 10].x, &chainCoordinates[currentLine - 10].y, &chainCoordinates[currentLine - 10].z);
		}

		if (currentLine == (nAtoms + 10))
		{
			currentArrayCounter = 0;

			for (int j = 0; j < nChains; ++j)
			{
				chainRGs[currentTimestep][j].x = 0;
				chainRGs[currentTimestep][j].y = 0;
				chainRGs[currentTimestep][j].z = 0;
			}

			for (int j = 0; j < nChains; ++j)
			{
				for (int k = 0; k < degreeOfPolymerization; ++k)
				{
					chainRGs[currentTimestep][j].x = pow((chainCOMs[currentTimestep][j].x - chainCoordinates[currentArrayCounter].x),2);
					chainRGs[currentTimestep][j].y = pow((chainCOMs[currentTimestep][j].y - chainCoordinates[currentArrayCounter].y),2)
					chainRGs[currentTimestep][j].z = pow((chainCOMs[currentTimestep][j].z - chainCoordinates[currentArrayCounter].z),2)

					currentArrayCounter++;
				}

				chainCOMs[currentTimestep][j].x /= degreeOfPolymerization;
				chainCOMs[currentTimestep][j].y /= degreeOfPolymerization;
				chainCOMs[currentTimestep][j].z /= degreeOfPolymerization;
			}

			currentLine = 0;
			currentTimestep++;
		}
	}

	return chainRGs;
}

int main(int argc, char const *argv[]) //By default, agrv[0] is the program name
{
    FILE *input; //This is declaring a pointer called input of type 'File type'. 
    input = fopen (argv[1], "r"); //argv[1] is the input dump file name. It will open the file and assign it to the pointer input.

    int degreeOfPolymerization = atoi (argv[2]); // Since argv[2] is a string, atoi converts it to an integer and stores. 
    int nChains = atoi (argv[3]); //Similar as above.
    int nTimeframes = 0;
    int nAtoms = 0;

    // nAtoms = calculateNatoms(input);
    // nTimeframes = calculateNtimeframes(input, nAtoms);

    // printf("%d %d", nAtoms, nTimeframes);

    CENTER_OF_MASS **chainCOMs;
	chainCOMs = (CENTER_OF_MASS **) malloc (nTimeframes * sizeof (CENTER_OF_MASS *));

	for (int i = 0; i < nTimeframes; ++i) 
	{
		chainCOMs[i] = (CENTER_OF_MASS *) malloc (nChains * sizeof (CENTER_OF_MASS)); 
	}

	RG **chainRGs;
	chainRGs = (RG **) malloc (nTimeframes * sizeof (RG *));

	for (int i = 0; i < nTimeframes; ++i) 
	{
		chainRGs[i] = (RG *) malloc (nChains * sizeof (RG)); 
	}

   return (0);

}