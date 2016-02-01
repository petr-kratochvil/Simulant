#include "Controller.h"
#include "GUI.h"
#include "Game21/SSG21.h"

Controller::Controller(wchar_t * jsonGameDescription)
{
	// TODO throw exception
	const JSONValue* parsedJSON = JSON::Parse(jsonGameDescription);
	this->symbolSet = new SymbolSet(parsedJSON);
	const JSONObject& parsedJSONObject = parsedJSON->AsObject();
	const JSONArray& parsedJSONReelSets = parsedJSONObject.at(L"reelSets")->AsArray();
	this->spinSourceGenerator = new SSG21(this->symbolSet, parsedJSONReelSets);
	this->simulator = new Simulator(*this->spinSourceGenerator);
	delete parsedJSON;
}

Controller::~Controller()
{
	delete this->simulator;
	delete this->symbolSet;
	delete this->spinSourceGenerator;
}

void Controller::assignGUI(GUI* gui)
{
	this->gui = gui;
}

void Controller::userStartClicked()
{
	this->simulator->spinOneStart();
	this->gui->setNewSpin(this->simulator->getLastSpin());
}
