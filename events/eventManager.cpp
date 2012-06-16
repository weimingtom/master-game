#include "eventManager.hpp"
#include "../objects/components/CompTogglable.hpp"
#include "../objects/components/CompVisualSq.hpp"
#include "../entity.hpp"


//eventManager *eventManager::mInstance = NULL;

/*
static eventManager * eventManager::getInstance() {
  if(mInstance == NULL)
	  mInstance = new eventManager();
  return mInstance;
	//return NULL;
}
*/

int handleButtonToggle(gameEvent evt)
{
    std::string target = evt.args["target"];
    CompTogglable* buttonState =  static_cast<CompTogglable*>(gameObjectsTable[target]->getComponent("CompTogglable"));
    CompVisualSq* vis =  static_cast<CompVisualSq*>(gameObjectsTable[target]->getComponent("CompVisualSq"));
    //tgtPos->pos.first=tgtPos->pos.first+1;
    buttonState->state=(buttonState->state ? 0 : 1);
    vis->sprite=(buttonState->state ? 3 : 4);
    std::cout << "Button pressed \n";
    return 0;
}


/*
int handleMoveFun(gameEvent evt)
{
    std::string target = evt.args["target"];
    CompPhys* tgtPos =  static_cast<CompPhys*>(gameObjectsTable[target]->getComponent("CompPhys"));
    tgtPos->pos.first=tgtPos->pos.first+1;
    std::cout << "Guest moved \n";
    return 0;
}
*/


void registerEvents()
{
    eventManager::eventHandler e1 = eventManager::eventHandler::from_function<&handleButtonToggle>();
    eventManager::getInstance()->registerHandler(e1,"handleButtonToggle","none");
    eventManager::eventNotifier n1;
    eventManager::getInstance()->registerNotifier(n1,"toggleNotifier","none");
    //eventManager::getInstance()->Subscribe("toggleEvent","handleButtonToggle","button");
};



void testEventManager()
{
    eventManager::eventHandler e1 = eventManager::eventHandler::from_function<&handleButtonToggle>();
    eventManager::getInstance()->registerHandler(e1,"handleButtonToggle","button");
    eventManager::eventNotifier n1;
    eventManager::getInstance()->registerNotifier(n1,"toggleNotifier","button");
    eventManager::getInstance()->Subscribe("toggleEvent","handleButtonToggle","button");
    gameEvent evt;
    evt.args.insert(std::pair<std::string,eventArg>("sender","Map"));
    evt.args.insert(std::pair<std::string,eventArg>("target","button"));
    //e1(evt);
    eventManager::getInstance()->Notify("toggleEvent","button",evt);
};
