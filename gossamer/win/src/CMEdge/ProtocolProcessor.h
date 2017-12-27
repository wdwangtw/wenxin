
#ifndef _PROTOCOL_PROCESSOR_H_
#define _PROTOCOL_PROCESSOR_H_

#include "gsoap\Processor.h"

class CProtocolProcessor : public CProcessor
{
protected:
    CProtocolProcessor();
    virtual ~CProtocolProcessor();

public:
    virtual int Dispatch(const std::string& request, std::string& response);

protected:
    int PackJSONP(const std::string& request, std::string& response);

protected:

};

#endif // !_PROTOCOL_PROCESSOR_H_
