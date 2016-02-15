#include "Controller.h"
#include "GUI.h"
#include "Game17/SSG17.h"
#include "Game21/SSG21.h"

// TODO get rid of dependence on other game libs
Controller::Controller(wchar_t * jsonGameDescription)
{
	// TODO throw exception
	const JSONValue* parsedJSON = JSON::Parse(jsonGameDescription);
	this->symbolSet = new SymbolSet(parsedJSON);
	const JSONObject& parsedJSONObject = parsedJSON->AsObject();
	const JSONArray& parsedJSONReelSets = parsedJSONObject.at(L"reelSets")->AsArray();
	
	int gameId = (int)parsedJSONObject.at(L"gameId")->AsNumber();
	switch (gameId)
	{
	case 17:
		{
			const JSONArray& parsedPayLines = parsedJSONObject.at(L"payLines")->AsArray();
			this->spinSourceGenerator = new SSG17(this->symbolSet, parsedJSONReelSets, parsedPayLines);
		}
		break;
	case 21:
		this->spinSourceGenerator = new SSG21(this->symbolSet, parsedJSONReelSets);
		break;
		// TODO throw a bunch of exceptions
	}

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
