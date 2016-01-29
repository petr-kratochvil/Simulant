#include <stdio.h>
#include "SimulantCore/JSON/JSON.h"
#include "SimulantCore/ReelSet.h"
#include "SimulantCore/SpinSourceGeneratorSeq.h"
#include <algorithm>

void getJSONInput(wchar_t* json, char* fileName)
{
	FILE *fr;
	errno_t err = fopen_s(&fr, fileName, "rt,ccs=UTF-8");

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

int cnts[9][9][9]; // symbol with ID=8 means that nothing came (X could not came)
int matrix[730][730];
int XSymbolCnt = 0;

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

bool idMakesSense(int id)
{
	if (id == 729)
		return true;
	int a = id % 9;
	id /= 9;
	int b = id % 9;
	id /= 9;
	int c = id;

	if (a == 8)
	{
		if ((b != 8) || (c != 8))
			return false;
	}
	else if (b == 8)
	{
		if (c != 8)
			return false;
	}
	else
	{
		if ((a == b) || (a == c) || (b == c))
			return false;
	}
	if ((a == 7) || (b == 7) || (c == 7))
		return false;
	return true;
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
	//printf("%d, %d, %d, %d -- ", result[0], result[1], result[2], result[3]);
	if (result[3] != 8)
		return 729;
	return getMatrixID(result[0], result[1], result[2]);
}

int transitionX(int a, int b, int c)
{
	if ((a != 8) && (b == 8))
		a = 8;
	else if ((a != 8) && (b != 8) && (c == 8))
		b = 8;
	else if ((a != 8) && (b != 8) && (c != 8))
		c = 8;
	return getMatrixID(a, b, c);
}

void computeMatrix(int a, int b, int c)
{
	int id = getMatrixID(a, b, c);
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			for (int k = 0; k < 9; k++)
			{
				matrix[id][transition(a, b, c, i, j, k)] += cnts[i][j][k];
			}
}

int main(int argc, char** argv)
{
	wchar_t* json = new wchar_t[10000];
	char fileName[50];
	if (argc <= 1)
		strcpy(fileName, "game21.json");
	else
		strcpy(fileName, argv[1]);
	printf("Loading input file %s\n", fileName);
	getJSONInput(json, fileName);
	const JSONValue* parsedJSON = JSON::Parse(json);
	
	SymbolSet sset(parsedJSON);

	JSONObject parsedJSONObject = parsedJSON->AsObject();
	JSONArray parsedJSONReelSets = parsedJSONObject[L"reelSets"]->AsArray();

	ReelSet rs(parsedJSONReelSets[0], sset);

	SpinSourceGeneratorSeq ssg(&sset, parsedJSONReelSets);

	delete [] json;

	memset(cnts, 0, sizeof(cnts));
	memset(matrix, 0, sizeof(matrix));
	
	int seq = 0;
	while (!ssg.seqEnded())
	{
		printf("\r %d", ++seq);
		Spin* spin = ssg.getNextSpin();
		const Window& w = spin->getWindow();
		std::vector<const Symbol*> winSymbols;
		bool wasXSymbol = false;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				for (int k = 0; k < 3; k++)
			{
				const Symbol& s1 = w.getSymbol(0, i);
				const Symbol& s2 = w.getSymbol(1, j);
				const Symbol& s3 = w.getSymbol(2, k);
				if ((&s1 == &s2) && (&s1 == &s3))
				{
					if (std::find(winSymbols.begin(), winSymbols.end(), &s1) == winSymbols.end())
						winSymbols.push_back(&s1);
				}
				if ((s1.getId() == 7) || (s2.getId() == 7) || (s3.getId() == 7))
					wasXSymbol = true;
			}
		if (wasXSymbol)
			XSymbolCnt++;
		for (int i = winSymbols.size() - 1; i < 3; i++)
			winSymbols.push_back(NULL);
		int first, second, third;
		if (winSymbols[0])
			first = winSymbols[0]->getId();
		else first = 8;
		if (winSymbols[1])
			second = winSymbols[1]->getId();
		else second = 8;
		if (winSymbols[2])
			third = winSymbols[2]->getId();
		else third = 8;
		cnts[first][second][third]++;
		delete spin;
	}

	/*for (int x = 0; x < 10; x++)
	{
		int a, b, c, i, j, k;
		scanf("%d%d%d%d%d%d", &a, &b, &c, &i, &j, &k);
		printf("%d\n", transition(a, b, c, i, j, k));
	}*/

	int counter = 1;
	FILE* fwIds = fopen("matrixIds.csv", "w");
	fprintf(fwIds, "Order\tID\tFirst\tSecond\tThird\n");
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			for (int k = 0; k < 9; k++)
			{
				computeMatrix(i, j, k);
				int id = getMatrixID(i, j, k);
			//	matrix[id][transitionX(i, j, k)] += XSymbolCnt;
				if (idMakesSense(id))
				{
					fprintf(fwIds, "%d\t%d\t%d\t%d\t%d\n", counter++, id, i, j, k);
				}
			}
	fclose(fwIds);
	matrix[729][729] = seq;

	JSONArray array;
	/*array.push_back(new JSONValue(1.0));
	array.push_back(new JSONValue(2.0));
	array.push_back(new JSONValue(3.0));
	JSONValue* value = new JSONValue(array);
	wprintf(L"%s\n", value->Stringify().c_str());*/

	FILE* fwMatrixTxt = fopen("matrix.txt", "w");
	for (int i = 0; i < 730; i++)
	{
		if (!idMakesSense(i))
			continue;
		JSONArray* inner = new JSONArray;
		int checksum = 0;
		for (int j = 0; j < 730; j++)
		{
			if (!idMakesSense(j))
				continue;
			if (matrix[i][j] > 0)
				;//__debugbreak();
			JSONValue* value = new JSONValue(double(matrix[i][j]));
			inner->push_back(value);
			checksum += matrix[i][j];
			double element = double(matrix[i][j]) / double(seq);
			fprintf(fwMatrixTxt, "%10.8f", element);
			if (j < 729)
				fprintf(fwMatrixTxt, " ");
		}
		fprintf(fwMatrixTxt, "\n");
		if (checksum != seq)
		{
			printf("Chyba, checksum radku %d je %d misto %d!\n", i, checksum, seq);
			__debugbreak();
		}
		JSONValue* value = new JSONValue(*inner);
		JSONObject* object = new JSONObject();
		object->insert(std::pair<const std::wstring, JSONValue*>(L"order", new JSONValue(double(i))));
		object->insert(std::pair<const std::wstring, JSONValue*>(L"row", value));
		JSONValue* value2 = new JSONValue(/*object*/ *value);
		array.push_back(value2);
	}
	fclose(fwMatrixTxt);

	FILE* fw = fopen("matrix.json", "wt,ccs=UTF-8");
	printf("Delka pole: %d\n", array.size());
	JSONValue* value = new JSONValue(array);
	fwprintf(fw, L"%s", value->Stringify(true).c_str());
	fclose(fw);

	FILE* fwMatrixZero = fopen("matrixZero.txt", "w");

	double** matrixZero = new double*[730];
	for (int i = 0; i < 730; i++)
	{
		matrixZero[i] = new double[730];
		memset(matrixZero[i], 0, sizeof(matrixZero[i]));
	}
	for (int i = 0; i < 730; i++)
		for (int j = 0; j < 730; j++)
			matrixZero[i][j] = 0;

	for (int i = 0; i < 730; i++)
		matrixZero[i][i] = 1.0;

	///////////////////////////////////////////////////////
	// probability of X symbol appearing on the middle reel
	double pX = 0.1;

	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			for (int k = 0; k < 9; k++)
			{
				int id1 = getMatrixID(i, j, k);
				int id2 = transitionX(i, j, k);
				if (id1 != id2)
				{
					matrixZero[id1][id2] = pX;
					matrixZero[id1][id1] -= pX;
				}
			}

	for (int i = 0; i < 730; i++)
	{
		if (!idMakesSense(i))
			continue;
		for (int j = 0; j < 730; j++)
		{
			if (!idMakesSense(j))
				continue;
			fprintf(fwMatrixZero, "%10.8f", matrixZero[i][j]);
			if (j < 729)
				fprintf(fwMatrixZero, " ");
		}
		fprintf(fwMatrixZero, "\n");
	}
	fclose(fwMatrixZero);
		
	return 0;
}