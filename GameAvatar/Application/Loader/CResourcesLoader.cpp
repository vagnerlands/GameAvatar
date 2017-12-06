#include "CResourcesLoader.h"
#include "CGameCockpit.h"

void CResourcesLoader::execute()
{
	// process the next item in the list until the list is empty...
	shared_ptr<IProcess> nextProcess;
	while ((nextProcess = CGameCockpit::instance()->getNextProcess()) != NULL)
	{
		nextProcess->load();
		CGameCockpit::instance()->popProcess(nextProcess);
		// perform backgroun updates in the game state
		CGameCockpit::instance()->Update();
	}
}

CResourcesLoader::~CResourcesLoader()
{
	// no implementation yet
}
