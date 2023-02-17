#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int findFirstTimeframe (FILE *input)
{
	char lineString[2000];
	int firstTimeframe;

	while (fgets (lineString, 2000, input) != NULL)
	{
		if (lineString[0] != '#')
		{
			sscanf (lineString, "%d\n", &firstTimeframe);
			return firstTimeframe;
		}
	}
}

int findLastTimeframe (const char *inputfilename)
{
	int lastTimeframe;
	char *pipeString, lineString[2000];
	pipeString = (char *) malloc (200 * sizeof (char));
	snprintf (pipeString, 200, "tail -1 %s", inputfilename);

	FILE *lastLine;
	lastLine = popen (pipeString, "r");
	fgets (lineString, 2000, lastLine);
	sscanf (lineString, "%d\n", &lastTimeframe);

	return lastTimeframe;
}

typedef struct responseData
{
	int timestep;
	float pressure, strain;
} RESPONSE_DATA;

RESPONSE_DATA *loadData (FILE *input, RESPONSE_DATA *inputData)
{
	rewind (input);

	char lineString[2000];
	int arrayLine = 0;

	while (fgets (lineString, 2000, input) != NULL)
	{
		if (lineString[0] != '#')
		{
			sscanf (lineString, "%d %f %f\n", &inputData[arrayLine].timestep, &inputData[arrayLine].pressure, &inputData[arrayLine].strain);
			arrayLine++;
		}
	}

	rewind (input);
	return (inputData);
}

int countTotalLines (const char *inputfilename)
{
	int totalLines;

	FILE *countLines;
	char *pipeString, lineString[2000];
	pipeString = (char *) malloc (200 * sizeof (char));
	snprintf (pipeString, 200, "wc -l %s", inputfilename);
	countLines = popen (pipeString, "r");

	fgets (lineString, 2000, countLines);
	sscanf (lineString, "%d\n", &totalLines);

	return totalLines;
}

int main(int argc, char const *argv[])
{
	if (argc == 1) {
		(void)printf("\nARGUMENTS:\n~~~~~~~~~~\n\n   {~} argv[0] = program\n   {~} argv[1] = input response file\n   {~} argv[2] = Time period of oscillation\n  \n\nHOW THIS WORKS?\n~~~~~~~~~~~~~~~\n\n   The program will perform simple averaging over all the oscillatory cycles\n\n");
		exit (1); }


	FILE *input;
	input = fopen (argv[1], "r");

	int timePeriodOfOscillation = atoi (argv[2]); 

	int firstTimeframe = findFirstTimeframe (input);

	int lastTimeframe = findLastTimeframe (argv[1]);

	int nPoints = (lastTimeframe - firstTimeframe) / timePeriodOfOscillation;

	int totalLines = countTotalLines (argv[1]);

	RESPONSE_DATA *inputData;
	inputData = (RESPONSE_DATA *) malloc (totalLines * sizeof (RESPONSE_DATA));

	inputData = loadData (input, inputData);

	// At this point, the data from input file is loaded. You can check the RAM by running it. It will work fine.
	// From this point, try to do averaging as you did in python.
	// If you can't get it done, I'll do it tomorrow night.

	fclose (input);
	return 0;
}