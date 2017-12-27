
#ifndef _GET_VERSION_PROCESSOR_H_
#define _GET_VERSION_PROCESSOR_H_

#include "ProtocolProcessor.h"
#include "EdgeBaseDefine.h"
#include "utils\cJSON.h"
#include <string>

class CGetVersionProcessor : public CProtocolProcessor
{
public:
    CGetVersionProcessor();
    ~CGetVersionProcessor();

    virtual int Dispatch(const std::string& request, std::string& response);

};

#endif // !_GET_VERSION_PROCESSOR_H_
