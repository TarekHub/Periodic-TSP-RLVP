#include "INCLUDE/MOW-P.h"

static char Delimiters[] = "= \n\t\r\f\v\xef\xbb\xbf";
static void Read_MU(void);
static void Read_Optimum(void);
static void Read_Lambda(void);
static void Read_MAX_ITERATIONS(void);
static void Read_MAX_TIME(void);
static void Read_START_TIME(void);
static void Read_TIME_HORIZON(void);
static void Read_MAX_DAILY_DURATION(void);
static void Read_LOADING(void);
static void Read_PRECISION();
static char *GetFileName(char *Line);

void ReadParameters()
{
    int i;
    char *Line, *Keyword, *Token;

    CurrentQuality = 0;
    Iteration = 0;

    if (ParameterFileName) {
        if (!(ParameterFile = fopen(ParameterFileName, "r")))
            eprintf("Cannot open PARAMETER_FILE: \"%s\"",
                    ParameterFileName);
        printff("PARAMETER_FILE = %s\n", ParameterFileName);
    } else{
        eprintf("PARAMETER_FILE not specified");
    }

    while ((Line = ReadLine(ParameterFile))) {
        if (!(Keyword = strtok(Line, Delimiters)))
            continue;
        if (Keyword[0] == '#')
            continue;
        for (i = 0; i < strlen(Keyword); i++)
            Keyword[i] = (char) toupper(Keyword[i]);
        if (!strcmp(Keyword, "MU")) {
          Read_MU();
        }else if (!strcmp(Keyword, "STRATEGY")){
			if (!(Token = strtok(0, Delimiters)))
				eprintf("%s", "STRATEGY: SW, ID_BEST, ID_ANY or GREEDY expected");

            for (i = 0; i < strlen(Token); i++)
                Token[i] = (char) toupper(Token[i]);

            if (!strncmp(Token, "SW", strlen(Token)))
                Strategy = SW;
            else if (!strncmp(Token, "ID_BEST", strlen(Token)))
                Strategy = ID_BEST;
            else if (!strncmp(Token, "ID_ANY", strlen(Token)))
                Strategy = ID_ANY;
            else if (!strncmp(Token, "GREEDY", strlen(Token)))
                Strategy = GREEDY;
			else
				eprintf("%s", "STRATEGY: SW, ID_BEST, ID_ANY or GREEDY expected");

		}else if (!strcmp(Keyword, "LAMBDA")) {
          Read_Lambda();
        }else if (!strcmp(Keyword, "MAX_ITERATIONS")) {
          Read_MAX_ITERATIONS();
        }else if (!strcmp(Keyword, "MAX_TIME")) {
          Read_MAX_TIME();
        }else if (!strcmp(Keyword, "START_TIME")) {
          Read_START_TIME();
        }else if (!strcmp(Keyword, "TIME_HORIZON")) {
          Read_TIME_HORIZON();
        }else if (!strcmp(Keyword, "PRECISION")) {
          Read_PRECISION();
        }else if(!strcmp(Keyword, "OPTIMUM")) {
            Read_Optimum();
        }else if(!strcmp(Keyword, "MAX_DAILY_DURATION")) {
          Read_MAX_DAILY_DURATION();
        }else if(!strcmp(Keyword, "LOADING")){
          Read_LOADING();
        }else if (!strcmp(Keyword, "PROBLEM_FILE")) {
            if (!(ProblemFileName = GetFileName(0)))
                eprintf("PROBLEM_FILE: string expected");
        }else if (!strcmp(Keyword, "INIT_TOURS_FILE")) {
            if (!(InitToursFileName = GetFileName(0)))
                eprintf("PROBLEM_FILE: string expected");
        }else if (!strcmp(Keyword, "OUTPUT_TOURS_FILE")) {
            if (!(OutputToursFileName = GetFileName(0)))
                eprintf("PROBLEM_FILE: string expected");
        }
    }

	fclose(ParameterFile);
    free(LastLine);
    LastLine = 0;
}

static void Read_Optimum()
{
    char *Token = strtok(0, Delimiters);

    if (!Token || !sscanf(Token, "%lf", &Optimum))
        eprintf("Optimum: float expected");
    if (Optimum < 0)
        eprintf("Optimum: < 0");
}

static void Read_MU()
{
    char *Token = strtok(0, Delimiters);

    if (!Token || !sscanf(Token, "%d", &muSize))
        eprintf("MU: Integer expected");
    if (muSize < 0)
        eprintf("MU: < 0");
}

static void Read_Lambda()
{
    char *Token = strtok(0, Delimiters);

    if (!Token || !sscanf(Token, "%d", &Lambda))
        eprintf("LAMBDA: Integer expected");
    if (Lambda < 0)
        eprintf("LAMBDA: < 0");
}

static void Read_MAX_ITERATIONS()
{
    char *Token = strtok(0, Delimiters);

    if (!Token || !sscanf(Token, "%d", &MaxIterations))
        eprintf("MAX_ITERATIONS: Integer expected");
    if (MaxIterations < 0)
        eprintf("MAX_ITERATIONS: < 0");
}

static void Read_MAX_TIME()
{
    char *Token = strtok(0, Delimiters);

    if (!Token || !sscanf(Token, "%d", &MaxTime))
        eprintf("MAX_TIME: Integer expected");
    if (MaxTime < 0)
        eprintf("MAX_TIME: < 0");
}

static void Read_START_TIME()
{
    char *Token = strtok(0, Delimiters);

    if (!Token || !sscanf(Token, "%lf", &StartTime))
        eprintf("START_TIME: long_float expected");
    if (StartTime < 0)
        eprintf("START_TIME: < 0");
}

static void Read_PRECISION()
{
    char *Token = strtok(0, Delimiters);

    if (!Token || !sscanf(Token, "%lf", &Precision))
        eprintf("PRECISION: Integer expected");
    if (MaxIterations < 0)
        eprintf("PRECISION: < 0");
}

static void Read_MAX_DAILY_DURATION()
{
    char *Token = strtok(0, Delimiters);

    if (!Token || !sscanf(Token, "%d", &MaxDailyDuration))
        eprintf("MAX_DAILY_DURATION(: Integer expected");
    if (MaxDailyDuration < 0)
        eprintf("MAX_DAILY_DURATION(: < 0");
}

static void Read_LOADING()
{
      char *Token = strtok(0, Delimiters);

    if (!Token || !sscanf(Token, "%d", &Loading))
        eprintf("LOADING(: Integer expected");
    if (Loading < 0)
        eprintf("LOADING: < 0");
}

static void Read_TIME_HORIZON()
{
    char *Token = strtok(0, Delimiters);

    if (!Token || !sscanf(Token, "%d", &TimeHorizon))
        eprintf("TIME_HORIZON: Integer expected");
    if (TimeHorizon < 0)
        eprintf("TIME_HORIZON: < 0");
}

static char *GetFileName(char *Line)
{
    char *Rest = strtok(Line, "\n\t\r\f"), *t;

    if (!Rest)
        return 0;
    while (isspace(*Rest))
        Rest++;
    if (!Line) {
        if (*Rest == '=')
            Rest++;
    }
    while (isspace(*Rest))
        Rest++;
    for (t = Rest + strlen(Rest) - 1; isspace(*t); t--)
        *t = '\0';
    if (!strlen(Rest))
        return 0;
    t = (char *) malloc(strlen(Rest) + 1);
    strcpy(t, Rest);
    return t;
}