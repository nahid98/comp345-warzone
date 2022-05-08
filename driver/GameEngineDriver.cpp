#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include "headers/GameEngine.h"
#include "headers/GameEngineDriver.h"
using namespace std;
#include <iostream>


 int GameEngineDriver() {
	 GameEngine* engine = new GameEngine();

	 engine->game_run();

	 delete engine;

	 _CrtDumpMemoryLeaks();
	 return 0;
 }