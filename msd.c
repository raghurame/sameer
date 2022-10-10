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

typedef struct centerOfMass
{
	float x, y, z;
} CENTER_OF_MASS;

typedef struct coordinates
{
	float x, y, z;
} COORDINATES;

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

// CHAIN_ENDS *fetchChainEnds (CHAIN_ENDS *ends, FILE *input, int nAtoms, int degreeOfPolymerization, int nChains)
// {
// 	char lineString[2000];
// 	int currentLine = 0, currentTimestep = 0;

// 	while (fgets (lineString, 2000, input) != NULL)
// 	{
// 		currentLine++;

// 		if (currentLine > 9 && currentLine <= 9 + nAtoms)
// 		{
// 			sscanf ()
// 		}
// 	}

// 	return ends;
// }

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

float calculateDistance (float x1, float y1, float z1, float x2, float y2, float z2)
{
	float distance;

	distance = sqrt (pow ((x2 - x1), 2) + pow ((y2 - y1), 2) + pow ((z2 - z1), 2));

	return distance;
}

float **computeMeanSquareDisplacement (float **meanSquareDisplacement, CENTER_OF_MASS **chainCOMs, int nTimeframes, int nChains)
{
	float distance;
	int delT;

	for (int currentChain = 0; currentChain < nChains; ++currentChain)
	{
		for (int initTime = 0; initTime < nTimeframes; ++initTime)
		{
			for (int finalTime = initTime; finalTime < nTimeframes; ++finalTime)
			{
				distance = calculateDistance (chainCOMs[initTime][currentChain].x, chainCOMs[initTime][currentChain].y, chainCOMs[initTime][currentChain].z, chainCOMs[finalTime][currentChain].x, chainCOMs[finalTime][currentChain].y, chainCOMs[finalTime][currentChain].z);				
				delT = finalTime - initTime;
				meanSquareDisplacement[delT][currentChain] = pow (distance, 2);
			}
		}
	}

	return meanSquareDisplacement;
}

float *avgMSD (float **meanSquareDisplacement, float *meanSquareDisplacement_avg, int nTimeframes, int nChains)
{
	// Initialize the values to zero
	for (int i = 0; i < nTimeframes; ++i)
	{
		meanSquareDisplacement_avg[i] = 0;
	}

	for (int i = 0; i < nTimeframes; ++i)
	{
		for (int j = 0; j < nChains; ++j)
		{
			meanSquareDisplacement_avg[i] += meanSquareDisplacement[i][j];
		}

		meanSquareDisplacement_avg[i] /= nChains;
	}
	return meanSquareDisplacement_avg;
}

int main(int argc, char const *argv[])
{
	FILE *input;
	input = fopen (argv[1], "r");

	int degreeOfPolymerization = atoi (argv[2]), nChains = atoi (argv[3]);

	int nAtoms = calculateNatoms (input), nTimeframes = calculateNtimeframes (input, nAtoms);

	// CHAIN_ENDS *ends;
	// ends = (CHAIN_ENDS *) malloc (nChains * sizeof (CHAIN_ENDS));

	// ends = fetchChainEnds (ends, input, nAtoms, degreeOfPolymerization, nChains);

	CENTER_OF_MASS **chainCOMs;
	chainCOMs = (CENTER_OF_MASS **) malloc (nTimeframes * sizeof (CENTER_OF_MASS *));

	for (int i = 0; i < nTimeframes; ++i) {
		chainCOMs[i] = (CENTER_OF_MASS *) malloc (nChains * sizeof (CENTER_OF_MASS)); }

	chainCOMs = computeCenterOfMass (chainCOMs, input, nAtoms, degreeOfPolymerization, nChains);

	float **meanSquareDisplacement;
	meanSquareDisplacement = (float **) malloc (nTimeframes * sizeof (float *));

	for (int i = 0; i < nTimeframes; ++i) {
		meanSquareDisplacement[i] = (float *) malloc (nChains * sizeof (float)); }

	meanSquareDisplacement = computeMeanSquareDisplacement (meanSquareDisplacement, chainCOMs, nTimeframes, nChains);

	float *meanSquareDisplacement_avg;
	meanSquareDisplacement_avg = (float *) malloc (nTimeframes * sizeof (float));

	meanSquareDisplacement_avg = avgMSD (meanSquareDisplacement, meanSquareDisplacement_avg, nTimeframes, nChains);

	// Testing
	for (int i = 0; i < nTimeframes; ++i)
	{
		printf("%d %f\n", i, meanSquareDisplacement_avg[i]);
		usleep (100000);
	}

	fclose (input);
	return 0;
}