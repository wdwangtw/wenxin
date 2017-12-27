#include "GetVersionProcessor.h"

CGetVersionProcessor::CGetVersionProcessor()
{
}

CGetVersionProcessor::~CGetVersionProcessor()
{
}

int CGetVersionProcessor::Dispatch(const std::string& request, std::string& response)
{
    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, GSOAP_JSON_FIELD_RESULT, 0);
    cJSON_AddNumberToObject(root, GSOAP_JSON_FIELD_VERSION, 1001);

    char* data = cJSON_Print(root);
    response = data;

    free(data);
    cJSON_Delete(root);

    PackJSONP(request, response);

    return EC_Succeed;
}
