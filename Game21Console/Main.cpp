#include "SimulantCore/Simulator.h"
#include "SimulantCore/SpinSourceGeneratorSeq.h"

void getJSONInput(wchar_t* json)
{
	FILE *fr;
	errno_t err = fopen_s(&fr, "game21.json", "rt,ccs=UTF-8");

	wchar_t buffer[10000];
	json[0] = '\0';
	int jsonLength = 0;
	while (!feof(fr))
	{
		fgetws(buffer, 10000, fr);
		wcsncat_s(json, 9999, buffer, 10000 - jsonLength);
		jsonLength += wcslen(buffer);
	}
}

int main()
{
	wchar_t json[10000];
	getJSONInput(json);
	const JSONValue* parsedJSON = JSON::Parse(json);
	SymbolSet sset(parsedJSON);

	JSONObject parsedJSONObject = parsedJSON->AsObject();
	JSONArray parsedJSONReelSets = parsedJSONObject[L"reelSets"]->AsArray();

	SpinSourceGeneratorSeq* sgSeq = new SpinSourceGeneratorSeq(&sset, parsedJSONReelSets);
	Simulator simulator(*sgSeq);

	int totalWin = 0;
	int64_t totalWinSquared = 0;
	int zeroCount = 0;
	int totalCount = 0;
	while (!sgSeq->seqEnded())
	{
		simulator.spinOneStart();
		const Spin& spin = simulator.getLastSpin();
		const Window& w = spin.getWindow();
		if (totalCount == 46312)
			__debugbreak();
		int win = w.winCrissCross3x3();
		totalCount++;
		totalWin += win;
		totalWinSquared += win*win;
		if (win == 0)
			zeroCount++;
		printf("\r%d", totalCount);
	}
	delete sgSeq;

	printf("Spin count: %d, zero count %d, i.e. %5.2f%%\n"
		, totalCount, zeroCount, double(zeroCount)/double(totalCount) * 100.0);
	printf("Total win: %d, RTP: %5.2f%%\n"
		, totalWin, double(totalWin) / double(5*totalCount) * 100.0);
	printf("Average win: %5.2f\n", double(totalWin) / double(totalCount-zeroCount));

	return 0;
}