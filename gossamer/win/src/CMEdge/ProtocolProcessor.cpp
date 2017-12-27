
#include "ProtocolProcessor.h"
#include "EdgeBaseDefine.h"
#include "utils\Comm.h"

CProtocolProcessor::CProtocolProcessor()
{
}

CProtocolProcessor::~CProtocolProcessor()
{
}

int CProtocolProcessor::Dispatch(const std::string& request, std::string& response)
{
    return 0;
}

int CProtocolProcessor::PackJSONP(const std::string& request, std::string& response)
{
    std::string callback = GetKeyValueAsStr(request, GSOAP_FIELD_CALLBACK);
    if (0 == callback.length())
    {
        return 0;
    }

    size_t pos = callback.find("?");
    if (std::string::npos != pos)
    {
        callback.resize(pos);
    }

    callback += "(";

    Comm::GBToUTF8(response, response);
    response.insert(0, callback);
    response += ")";

    return 0;
}
