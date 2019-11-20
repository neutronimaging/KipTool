//<LICENSE>

#include "stdafx.h"

#include <sstream>
#include <iomanip>
#include <string>

#include <strings/miscstring.h>
#include <strings/string2array.h>
#include <strings/xmlstrings.h>

#include "../include/KiplProcessConfig.h"
#include "../include/KiplFrameworkException.h"

// Main config class

KiplProcessConfig::KiplProcessConfig(void) :
    ConfigBase("KiplProcessConfig")
{
}

KiplProcessConfig::~KiplProcessConfig(void)
{

}

std::string KiplProcessConfig::WriteXML()
{
	std::ostringstream str;

	int indent=4;
	str<<"<kiplprocessing>\n";
        str<<UserInformation.WriteXML(indent);
		str<<mSystemInformation.WriteXML(indent);
		str<<mImageInformation.WriteXML(indent);

		str<<mOutImageInformation.WriteXML(indent);

		if (!modules.empty()) {
			str<<std::setw(indent)<<" "<<"<processchain>\n";
			std::list<ModuleConfig>::iterator it;

			for (it=modules.begin(); it!=modules.end(); it++) {
				str<<it->WriteXML(indent+4);
			}
			str<<std::setw(indent)<<" "<<"</processchain>\n";
		}


		str<<"</kiplprocessing>"<<std::endl;

		return str.str();
}

void KiplProcessConfig::ParseConfig(xmlTextReaderPtr reader, std::string sName)
{
    if (sName=="system")
        mSystemInformation.ParseXML(reader);

    if (sName=="image")
        mImageInformation.ParseXML(reader);

    if (sName=="outimage")
        mOutImageInformation.ParseXML(reader);
}

void KiplProcessConfig::ParseProcessChain(xmlTextReaderPtr reader)
{
	const xmlChar *name, *value;
    int ret = xmlTextReaderRead(reader);
    std::string sName, sValue;
    int depth=xmlTextReaderDepth(reader);
	logger(kipl::logging::Logger::LogVerbose,"Parsing processchain");
    while (ret == 1) {
    	if (xmlTextReaderNodeType(reader)==1) {
	        name = xmlTextReaderConstName(reader);
	        ret=xmlTextReaderRead(reader);
	        
	        value = xmlTextReaderConstValue(reader);
            if (name==nullptr) {
				throw KiplFrameworkException("Unexpected contents in parameter file",__FILE__,__LINE__);
	        }
            if (value!=nullptr)
	        	sValue=reinterpret_cast<const char *>(value);
	        else
	        	sValue="Empty";
	        sName=reinterpret_cast<const char *>(name);

			
			int depth2=xmlTextReaderDepth(reader);
			if (sName=="module") {
                ModuleConfig module(m_sApplicationPath);
				module.ParseModule(reader);
				modules.push_back(module);
			}

			ret = xmlTextReaderRead(reader);
			if (xmlTextReaderDepth(reader)<depth2)
				ret=0;
		}
		ret = xmlTextReaderRead(reader);
		if (xmlTextReaderDepth(reader)<depth)
    		ret=0;
	}
}


// System information methods

KiplProcessConfig::cSystemInformation::cSystemInformation(): 
	nMemory(1500ul),
	eLogLevel(kipl::logging::Logger::LogMessage)
{}

KiplProcessConfig::cSystemInformation::cSystemInformation(const cSystemInformation &a) : 
	nMemory(a.nMemory), 
	eLogLevel(a.eLogLevel) 
{}

KiplProcessConfig::cSystemInformation & KiplProcessConfig::cSystemInformation::operator=(const cSystemInformation &a) 
{
	nMemory=a.nMemory; 
	eLogLevel=a.eLogLevel; 
	return *this;
}

std::string KiplProcessConfig::cSystemInformation::WriteXML(int indent)
{
	using namespace std;
	ostringstream str;
	
	str<<setw(indent)  <<" "<<"<system>"<<std::endl;
	str<<setw(indent+4)<<" "<<"<memory>"<<nMemory<<"</memory>"<<std::endl;
	str<<setw(indent+4)<<"  "<<"<loglevel>"<<eLogLevel<<"</loglevel>"<<std::endl;
	str<<setw(indent)  <<"  "<<"</system>"<<std::endl;

	return str.str();
}	

void KiplProcessConfig::cSystemInformation::ParseXML(xmlTextReaderPtr reader)
{
	const xmlChar *name, *value;
    int ret = xmlTextReaderRead(reader);
    std::string sName, sValue;
    int depth=xmlTextReaderDepth(reader);

    while (ret == 1) {
    	if (xmlTextReaderNodeType(reader)==1) {
	        name = xmlTextReaderConstName(reader);
	        ret=xmlTextReaderRead(reader);
	        
	        value = xmlTextReaderConstValue(reader);
            if (name==nullptr) {
	            throw KiplFrameworkException("Unexpected contents in parameter file",__FILE__,__LINE__);
	        }
            if (value!=nullptr)
	        	sValue=reinterpret_cast<const char *>(value);
	        else
	        	sValue="Empty";
	        sName=reinterpret_cast<const char *>(name);

	        if (sName=="memory") {
				nMemory=atoi(sValue.c_str());
	        }

	        if (sName=="loglevel") 
                string2enum(sValue,eLogLevel);
		}
        ret = xmlTextReaderRead(reader);
        if (xmlTextReaderDepth(reader)<depth)
        	ret=0;
    }
}


//
// Image information methods

KiplProcessConfig::cImageInformation::cImageInformation() :
		sSourcePath("./"),
		sSourceFileMask("slice_####.tif"),
		bUseROI(false),
		nFirstFileIndex(1),
        nLastFileIndex(100),
        nStepFileIndex(1),
        nStride(1),
        nRepeat(1),
        eFlip(kipl::base::ImageFlipNone),
        eRotate(kipl::base::ImageRotateNone)
{
	nROI[0]=nROI[1]=0;
	nROI[2]=nROI[3]=100;
}

KiplProcessConfig::cImageInformation::cImageInformation(const cImageInformation &a) :
	sSourcePath(a.sSourcePath),
	sSourceFileMask(a.sSourceFileMask),
	bUseROI(a.bUseROI),
	nFirstFileIndex(a.nFirstFileIndex),
    nLastFileIndex(a.nLastFileIndex),
    nStepFileIndex(a.nStepFileIndex),
    nStride(a.nStride),
    nRepeat(a.nRepeat),
    eFlip(a.eFlip),
    eRotate(a.eRotate)
{
	nROI[0]=a.nROI[0];
	nROI[1]=a.nROI[1];
	nROI[2]=a.nROI[2];
	nROI[3]=a.nROI[3];
}

KiplProcessConfig::cImageInformation & KiplProcessConfig::cImageInformation::operator=(const cImageInformation &a) 
{
	sSourcePath          = a.sSourcePath;
	sSourceFileMask      = a.sSourceFileMask;
	bUseROI              = a.bUseROI;
	nFirstFileIndex      = a.nFirstFileIndex;
	nLastFileIndex       = a.nLastFileIndex;
    nStride              = a.nStride;
    nRepeat              = a.nRepeat;
    eFlip                = a.eFlip;
    eRotate              = a.eRotate;

	nROI[0]=a.nROI[0];
	nROI[1]=a.nROI[1];
	nROI[2]=a.nROI[2];
	nROI[3]=a.nROI[3];

	return *this;
}

std::string KiplProcessConfig::cImageInformation::WriteXML(int indent)
{
	using namespace std;
	ostringstream str;
	
	str<<setw(indent)  <<" "<<"<image>"<<std::endl;
	str<<setw(indent+4)<<" "<<"<srcpath>"<<sSourcePath<<"</srcpath>"<<std::endl;
	str<<setw(indent+4)<<"  "<<"<srcfilemask>"<<sSourceFileMask<<"</srcfilemask>"<<std::endl;
	str<<setw(indent+4)<<"  "<<"<firstfileindex>"<<nFirstFileIndex<<"</firstfileindex>"<<std::endl;
	str<<setw(indent+4)<<"  "<<"<lastfileindex>"<<nLastFileIndex<<"</lastfileindex>"<<std::endl;
    str<<kipl::strings::xmlString("useroi", bUseROI,              indent+4);
    str<<kipl::strings::xmlString("stride", nStride,              indent+4);
    str<<kipl::strings::xmlString("repeat", nRepeat,              indent+4);
    str<<kipl::strings::xmlString("flip",   enum2string(eFlip),   indent+4);
    str<<kipl::strings::xmlString("rotate", enum2string(eRotate), indent+4);

	str<<setw(indent+4)<<"  "<<"<roi>"<<nROI[0]<<" "<<nROI[1]<<" "<<nROI[2]<<" "<<nROI[3]<<"</roi>"<<std::endl;
	str<<setw(indent)  <<"  "<<"</image>"<<std::endl;

	return str.str();
}	

void KiplProcessConfig::cImageInformation::ParseXML(xmlTextReaderPtr reader)
{
	const xmlChar *name, *value;
    int ret = xmlTextReaderRead(reader);
    std::string sName, sValue;
    int depth=xmlTextReaderDepth(reader);

    while (ret == 1) {
    	if (xmlTextReaderNodeType(reader)==1) {
	        name = xmlTextReaderConstName(reader);
	        ret=xmlTextReaderRead(reader);
	        
	        value = xmlTextReaderConstValue(reader);
            if (name==nullptr) {
	            throw KiplFrameworkException("Unexpected contents in parameter file",__FILE__,__LINE__);
	        }
            if (value!=nullptr)
	        	sValue=reinterpret_cast<const char *>(value);
	        else
	        	sValue="Empty";
	        sName=reinterpret_cast<const char *>(name);

	        if (sName=="srcpath") {
				sSourcePath=sValue;
	        }

			if (sName=="srcfilemask") {
				sSourceFileMask=sValue;
	        }


			if (sName=="useroi") {
				bUseROI=kipl::strings::string2bool(sValue);
			}

			if (sName=="roi") {
				kipl::strings::String2Array(sValue,nROI,4);
			}

			if (sName=="firstfileindex") {
				nFirstFileIndex=atoi(sValue.c_str());
			}

			if (sName=="lastfileindex") {
				nLastFileIndex=atoi(sValue.c_str());
			}

            if (sName=="stride")
            {
                nStride=std::stoul(sValue);
            }

            if (sName=="repeat")
            {
                nRepeat=std::stoul(sValue);
            }

            if (sName=="flip")
            {
                string2enum(sValue,eFlip);
            }
            if (sName=="rotate"){
                string2enum(sValue,eRotate);
            }

		}
        ret = xmlTextReaderRead(reader);
        if (xmlTextReaderDepth(reader)<depth)
        	ret=0;
    }
}

KiplProcessConfig::cOutImageInformation::cOutImageInformation() :
		bRescaleResult(false),
		eResultImageType(kipl::io::TIFF16bits),
		sDestinationPath(""),
		sDestinationFileMask("result_####.tif")
{

}

KiplProcessConfig::cOutImageInformation::cOutImageInformation(const KiplProcessConfig::cOutImageInformation &a) :
bRescaleResult(a.bRescaleResult),
eResultImageType(a.eResultImageType),
sDestinationPath(a.sDestinationPath),
sDestinationFileMask(a.sDestinationFileMask)
{}

KiplProcessConfig::cOutImageInformation & KiplProcessConfig::cOutImageInformation::operator=(const KiplProcessConfig::cOutImageInformation &a)
{
	bRescaleResult       = a.bRescaleResult;
	eResultImageType     = a.eResultImageType;
	sDestinationPath     = a.sDestinationPath;
	sDestinationFileMask = a.sDestinationFileMask;

	return *this;
}

std::string KiplProcessConfig::cOutImageInformation::WriteXML(int indent)
{
	using namespace std;
	ostringstream str;

	str<<setw(indent)  <<" "<<"<outimage>"<<std::endl;
	str<<setw(indent+4)<<" "<<"<dstpath>"<<sDestinationPath<<"</dstpath>"<<std::endl;
	str<<setw(indent+4)<<"  "<<"<dstfilemask>"<<sDestinationFileMask<<"</dstfilemask>"<<std::endl;
	str<<setw(indent+4)<<"  "<<"<rescaleresult>"<<kipl::strings::bool2string(bRescaleResult)<<"</rescaleresult>\n";
	str<<setw(indent+4)<<"  "<<"<imagetype>"<<eResultImageType<<"</imagetype>\n";
	str<<setw(indent)  <<"  "<<"</outimage>"<<std::endl;

	return str.str();

}

void KiplProcessConfig::cOutImageInformation::ParseXML(xmlTextReaderPtr reader)
{
	const xmlChar *name, *value;
    int ret = xmlTextReaderRead(reader);
    std::string sName, sValue;
    int depth=xmlTextReaderDepth(reader);

    while (ret == 1) {
    	if (xmlTextReaderNodeType(reader)==1) {
	        name = xmlTextReaderConstName(reader);
	        ret=xmlTextReaderRead(reader);

	        value = xmlTextReaderConstValue(reader);
            if (name==nullptr) {
	            throw KiplFrameworkException("Unexpected contents in parameter file",__FILE__,__LINE__);
	        }
            if (value!=nullptr)
	        	sValue=reinterpret_cast<const char *>(value);
	        else
	        	sValue="Empty";
	        sName=reinterpret_cast<const char *>(name);

	        if (sName=="dstpath") {
				sDestinationPath=sValue;
	        }

			if (sName=="dstfilemask") {
				sDestinationFileMask=sValue;
	        }

			if (sName=="rescaleresult") {
				bRescaleResult=kipl::strings::string2bool(sValue);
			}

			if (sName=="imagetype") {
                string2enum(sValue,eResultImageType);
			}
		}
        ret = xmlTextReaderRead(reader);
        if (xmlTextReaderDepth(reader)<depth)
        	ret=0;
    }
}





