#include <stdio.h>
#include "SimulantCore/JSON/JSON.h"
#include "SimulantCore/ReelSet.h"
#include "SimulantCore/SpinSourceGeneratorSeq.h"
#include <algorithm>

void getJSONInput(wchar_t* json)
{
	FILE *fr;
	errno_t err = fopen_s(&fr, "game21.json", "rt,ccs=UTF-8");

	wchar_t buffer[1000];
	json[0] = '\0';
	int jsonLength = 0;
	while (!feof(fr))
	{
		fgetws(buffer, 1000, fr);
		wcsncat_s(json, 999, buffer, 1000 - jsonLength);
		jsonLength += wcslen(buffer);
	}
}

int cnts[9][9][9]; // symbol with ID=7 means that nothing came (X could not came)
int matrix[730][730];

int getMatrixID(int a, int b, int c)
{
	int index = a + 9 * b + 9 * 9 * c;
	if (index >= 729)
	{
		printf("Error accessing matrix!\n");
		exit(1);
	}
	return index;
}

int transition(int a, int b, int c, int i, int j, int k)
{
	std::vector<int> result(0);
	std::vector<int> source = { a, b, c };
	std::vector<int> new_spin = { i, j, k };

	for (int s = 0; s < 3; s++)
	{
		if (source[s] != 8)
			result.push_back(source[s]);
		else
			break;
	}
	for (int ns = 0; ns < 3; ns++)
	{
		if (std::find(result.begin(), result.end(), new_spin[ns]) == result.end())
			result.push_back(new_spin[ns]);
	}
	int size = result.size();
	result.resize(4);
	for (int res = size; res < 4; res++)
		result[res] = 8;
	printf("%d, %d, %d, %d -- ", result[0], result[1], result[2], result[3]);
	if (result[3] != 8)
		return 729;
	return getMatrixID(result[0], result[1], result[2]);
}

void computeMatrix(int a, int b, int c)
{
	int id = getMatrixID(a, b, c);
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			for (int k = 0; k < 9; k++)
			{
				matrix[id][transition(a, b, c, i, j, k)]++;
			}
}

int main()
{
	wchar_t json[2000];
	getJSONInput(json);
	const JSONValue* parsedJSON = JSON::Parse(json);
	
	SymbolSet sset(parsedJSON);

	JSONObject parsedJSONObject = parsedJSON->AsObject();
	JSONArray parsedJSONReelSets = parsedJSONObject[L"reelSets"]->AsArray();

	ReelSet rs(parsedJSONReelSets[0], sset);

	SpinSourceGeneratorSeq ssg(&sset, parsedJSONReelSets);

	memset(cnts, 0, sizeof(cnts));
	memset(matrix, 0, sizeof(matrix));
	
	int seq = 0;
	while (!ssg.seqEnded())
	{
		printf("\r %d", seq++);
		Spin* spin = ssg.getNextSpin();
		const Window& w = spin->getWindow();
		std::vector<const Symbol*> winSymbols;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				for (int k = 0; k < 3; k++)
			{
				const Symbol& s1 = w.getSymbol(0, i);
				const Symbol& s2 = w.getSymbol(1, j);
				const Symbol& s3 = w.getSymbol(2, k);
				if ((&s1 == &s2) && (&s1 == &s3))
				{
					winSymbols.push_back(&s1);
				}
			}
		for (int i = winSymbols.size() - 1; i < 3; i++)
			winSymbols.push_back(NULL);
		int first, second, third;
		if (winSymbols[0])
			first = winSymbols[0]->getId();
		else first = 7;
		if (winSymbols[1])
			second = winSymbols[1]->getId();
		else second = 7;
		if (winSymbols[2])
			third = winSymbols[2]->getId();
		else third = 7;
		cnts[first][second][third]++;
		delete spin;
	}

/*	for (int x = 0; x < 10; x++)
	{
		int a, b, c, i, j, k;
		scanf("%d%d%d%d%d%d", &a, &b, &c, &i, &j, &k);
		printf("%d\n", transition(a, b, c, i, j, k));
	}*/
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			for (int k = 0; k < 9; k++)
				computeMatrix(i, j, k);

	JSONArray array;
	/*array.push_back(new JSONValue(1.0));
	array.push_back(new JSONValue(2.0));
	array.push_back(new JSONValue(3.0));
	JSONValue* value = new JSONValue(array);
	wprintf(L"%s\n", value->Stringify().c_str());*/
	for (int i = 0; i < 730; i++)
	{
		JSONArray* inner = new JSONArray;
		for (int j = 0; j < 730; j++)
		{
			JSONValue* value = new JSONValue(double(matrix[i][j]));
			inner->push_back(value);
		}
		JSONValue* value = new JSONValue(*inner);
		array.push_back(value);
	}

	FILE* fw = fopen("matrix.json", "wt,ccs=UTF-8");
	JSONValue* value = new JSONValue(array);
	fwprintf(fw, L"%s", value->Stringify().c_str());
	fclose(fw);

	return 0;
}