#include "eventManager.hpp"

eventManager *eventManager::mInstance = NULL;

eventManager * eventManager::getInstance() {
   if(mInstance == NULL)
	  mInstance = new eventManager();
   return mInstance;
}