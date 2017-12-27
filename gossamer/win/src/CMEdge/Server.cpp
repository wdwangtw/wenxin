
#include "Server.h"
#include "EdgeBaseDefine.h"
#include "DataModule.h"
#include "utils\LogService.h"
#include "utils\Comm.h"
#include <stdio.h>
#include <sstream>

CServer::CServer()
    :exit_event_(CreateEvent(0, TRUE, FALSE, NAME_EVENT_SERVICEEXIT))
{
}

CServer::~CServer()
{
}

void CServer::Run()
{
    if (Initialize())
    {
#ifndef RUN_AS_SERVICE
        char c;
        scanf("%c", &c);
#else
        WaitForSingleObject(exit_event_, INFINITE);
#endif
    }

    Uninitialize();
}

bool CServer::Initialize()
{
    CConfig config;
    LoadConfig(config);

    if (false == InitLogService(config))
    {
        return false;
    }

    if (false == InitEdgeAdapter(config))
    {
        return false;
    }

    if (false == InitPingAgent(config))
    {
        return false;
    }

    if (false == InitTaskService(config))
    {
        return false;
    }

    return true;
}

void CServer::Uninitialize()
{
    task_service_.Uninitialize();
    ping_agent_.Stop();
    edge_adapter_.Stop();

    CLogService::Instance()->Uninitialize();
    CLogService::CloseInstance();

    CDataModule::CloseInstance();
}

void CServer::LoadConfig(CConfig& config)
{
    std::string file;
    if (0 == Comm::GetModulePath(file))
    {
        return;
    }

    size_t pos = file.rfind('\\');
    if (-1 == pos)
    {
        return;
    }

    file.resize(pos + 1);
    file += NAME_CONFIG_FILE;

    config.Load(file);
}

bool CServer::InitLogService(const CConfig& config)
{
    return CLogService::Instance()->Initialize(config.GetLogDir(), config.GetLogFile(), static_cast<ELogLevel>(config.GetLogLevel()), config.GetMaxLogFileNum(), config.GetMaxLogLineNum());
}

bool CServer::InitEdgeAdapter(const CConfig& config)
{
    std::stringstream super_node_ip;
    super_node_ip << config.GetSuperNodeIP();
    super_node_ip << ":";
    super_node_ip << config.GetSuperNodePort();

    edge_adapter_.SetSuperNodeIP(super_node_ip.str());
    edge_adapter_.SetInterfaceName(config.GetInterfaceName());
    edge_adapter_.SetEdgeIP(config.GetEdgeIP());
    edge_adapter_.SetGroupName(config.GetGroupName());
    edge_adapter_.SetPassword(config.GetPassword());
    edge_adapter_.SetMacAddress(config.GetMacAddress());
    edge_adapter_.SetMask(config.GetMask());
    edge_adapter_.SetMTU(config.GetMTU());

    if (-1 == edge_adapter_.Start())
    {
        return false;
    }

    return true;
}

bool CServer::InitPingAgent(const CConfig& config)
{
    ping_agent_.Start(config.GetPingInterval(), config.GetPingDstAddress());
    
    return true;
}

bool CServer::InitTaskService(const CConfig& config)
{
    task_service_.SetConfigFile(config.GetVncPath());

    return task_service_.Initialize();
}
