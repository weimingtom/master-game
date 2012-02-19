

#ifndef __SLOTMGR_HPP__
#define __SLOTMGR_HPP__

#include <map>
#include "events.hpp"
#include <cstdio>
#include <list>

class slotMgr {
protected:

	typedef std::map<const comp_id_type, std::list<ComponentTemplate*> > comp_table_t;
    typedef std::map<const comp_id_type, rapidxml::xml_node<char>* > root_table_t;

	CompTemplateMgr() {}
public:
	~CompTemplateMgr() {}

	static CompTemplateMgr *getInstance() {
		if(mInstance == NULL)
			mInstance = new CompTemplateMgr();
		return mInstance;
	}

	void clearComponents() {
		comp_table_t::iterator iter;
		for(iter = mTemplates.begin(); iter != mTemplates.end(); iter++) {
			iter->second.clear();
		}
		mTemplates.clear();
	}

	void registerTemplate(ComponentTemplate *templ) {
		mTemplates[templ->componentID()].push_back(templ);
	}

	Component *createComp(comp_id_type& comp) {
	    printf("creating component \n");
		return mTemplates[comp].front()->makeComponent();
	}

    ComponentTemplate *getTemplate(comp_id_type& comp) {
        //поскольку шаблон должен где-то храниться, передвинем его в конец (циклическая перестановка)
        ComponentTemplate* t1= mTemplates[comp].front();
        mTemplates[comp].insert(mTemplates[comp].end(),t1);
        mTemplates[comp].erase(mTemplates[comp].begin());
        ComponentTemplate* t2= mTemplates[comp].back();
        //std::iter_swap(from, to);
        //mTemplates[comp].erase(mTemplates[comp].begin());
        //mTemplates[comp].pop_front();
		return t2;

	}
    rapidxml::xml_node<char> *getRootTemplateNode(comp_id_type& comp) {
        rapidxml::xml_node<char> *n1= mRootNodes[comp];
		return n1;

    }


    void registerRootTemplateNode(comp_id_type& comp,rapidxml::xml_node<char> *n1) {
        mRootNodes[comp]=n1;

    }




protected:
	comp_table_t mTemplates;
	root_table_t mRootNodes;

	static CompTemplateMgr *mInstance;
};

#endif

