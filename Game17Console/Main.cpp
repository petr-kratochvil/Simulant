#include <stdio.h>
#include "SimulantCore/Simulator.h"
#include "Game17/SSG17.h"
#include "SimulantCore/Random.h"

void getJSONInput(wchar_t* json, char* fileName)
{
	FILE *fr;
	errno_t err = fopen_s(&fr, fileName, "rt,ccs=UTF-8");

	wchar_t buffer[20000];
	json[0] = '\0';
	int jsonLength = 0;
	while (!feof(fr))
	{
		fgetws(buffer, 15000, fr);
		wcsncat_s(json, 19999, buffer, 20000 - jsonLength);
		jsonLength += wcslen(buffer);
	}
}

int main(int argc, char** argv)
{
	wchar_t json[20000];
	char fileName[50];
	if (argc <= 1)
		strcpy(fileName, "../resources/game17/game17.json");
	else
		strcpy(fileName, argv[1]);
	printf("Loading input file %s\n", fileName);
	getJSONInput(json, fileName);

	const JSONValue* parsedJSON = JSON::Parse(json);
	SymbolSet sset(parsedJSON);
	JSONObject parsedJSONObject = parsedJSON->AsObject();
	JSONArray parsedJSONReelSets = parsedJSONObject[L"reelSets"]->AsArray();
	JSONArray parsedJSONPayLines = parsedJSONObject[L"payLines"]->AsArray();

	int64_t totalWin = 0;
	int64_t totalWinSquared = 0;
	int zeroCount = 0;
	int totalCount = 0;

	Random::init();

	Statistics statistics(sset.getSymbolCount());

	if ((argc >= 3) &&
		strcmp(argv[2], "-seq") == 0)
	{
		printf("-seq not available so far.\n");
		return 0;
	}

	int iterations = 1e6;
	double it;
	if (argc >= 3)
	{
		sscanf(argv[2], "%lf", &it);
		iterations = int(it);
	}

	SpinSourceGenerator* sg = new SSG17(&sset, parsedJSONReelSets, parsedJSONPayLines);
	Simulator simulator(*sg);

	for (int i = 0; i < iterations; i++)
	{
		simulator.spinOneStart();
		const Spin& spin = simulator.getLastSpin();

		statistics.addSpin(spin);
		int win = spin.getTotalWin();
		totalCount++;
		totalWin += win;
		totalWinSquared += win*win;
		if (win == 0)
			zeroCount++;
		if ((totalCount % 100000) == 0)
			printf("\r%d %d00 000", totalCount / 1000000, (totalCount % 1000000) / 100000);
	}

	printf("Spin count: %d, zero count %d, i.e. %5.2f%%\n"
		, totalCount, zeroCount, double(zeroCount) / double(totalCount) * 100.0);
	printf("Total win: %lld, RTP: %5.2f%%\n"
		, totalWin, double(totalWin) / double(5 * totalCount) * 100.0);
	printf("Average win: %5.2f\n", double(totalWin) / double(totalCount - zeroCount));
	printf("\nStatistics:\n");
	wprintf(L"%s", statistics.getDescription().c_str());

	FILE* fw = fopen("statWinByBets", "w");
	fprintf(fw, "%s", statistics.getWinStatsByBets().c_str());
	fclose(fw);
	fw = fopen("statWinBasic", "w");
	fprintf(fw, "%s", statistics.getWinStatsBasic().c_str());
	fclose(fw);
	fw = fopen("statWinBonus", "w");
	fprintf(fw, "%s", statistics.getWinStatsBonus().c_str());
	fclose(fw);
}