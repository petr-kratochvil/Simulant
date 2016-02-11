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

	this->statistics = new Statistics(this->symbolSet->getSymbolCount(), 1000);
}

Controller::~Controller()
{
	delete this->simulator;
	delete this->symbolSet;
	delete this->spinSourceGenerator;
	delete this->statistics;
}

void Controller::assignGUI(GUI* gui)
{
	this->gui = gui;
}

void Controller::userStartClicked()
{
	this->simulator->spinOneStart();
	this->statistics->addSpin(this->simulator->getLastSpin());
	this->gui->setNewSpin(this->simulator->getLastSpin());
	this->updateDescription();
	this->gui->setGoBackEnabled(this->simulator->canGoBack());
}

void Controller::userBackClicked()
{
	if (this->simulator->canGoBack())
	{
		this->simulator->goBackOneSpin();
		this->gui->setNewSpin(this->simulator->getLastSpin());
	}
	this->gui->setGoBackEnabled(this->simulator->canGoBack());
}

void Controller::userBetUpClicked()
{
	this->simulator->betUp();
	this->updateDescription();
}

void Controller::userBetDownClicked()
{
	this->simulator->betDown();
	this->updateDescription();
}

void Controller::updateDescription()
{
	std::wstringstream ws;
	ws << L"Sázka:\t\t" << this->simulator->getBet() << L"\r\n";
	ws << this->statistics->getDescription();
	this->gui->setNewDescription(ws.str());
}
