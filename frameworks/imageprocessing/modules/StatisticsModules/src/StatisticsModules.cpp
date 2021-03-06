//<LICENSE>

// BaseModules.cpp : Defines the exported functions for the DLL application.
//

#include "statisticsmodules_global.h"
#include "ImageHistogram.h"

#include <KiplProcessModuleBase.h>

#include <cstdlib>
#include <map>
#include <list>
#include <string>


STATISTICSMODULESSHARED_EXPORT void * GetModule(const char *application, const char * name, void *vinteractor)
{
	if (strcmp(application,"kiptool")!=0)
        return nullptr;

        kipl::interactors::InteractionBase *interactor=reinterpret_cast<kipl::interactors::InteractionBase *>(vinteractor);
    if (name!=nullptr) {
		std::string sName=name;

		if (sName=="ImageHistogram")
			return new ImageHistogram;
	}

    return nullptr;
}

STATISTICSMODULESSHARED_EXPORT  int Destroy(const char *application, void *obj)
{
	if (strcmp(application,"kiptool")!=0)
		return -1;

    if (obj!=nullptr)
        delete reinterpret_cast<KiplProcessModuleBase *>(obj);

	return 0;
}

STATISTICSMODULESSHARED_EXPORT int LibVersion()
{
	return -1;
}

STATISTICSMODULESSHARED_EXPORT int GetModuleList(const char *application, void *listptr)
{
	if (strcmp(application,"kiptool")!=0)
		return -1;

	std::map<std::string, std::map<std::string, std::string> > *modulelist=reinterpret_cast<std::map<std::string, std::map<std::string, std::string> > *>(listptr);

	ImageHistogram mih;
	modulelist->operator []("ImageHistogram")=mih.GetParameters();

	return 0;
}

