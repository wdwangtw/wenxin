
#include "Config.h"
#include "utils\LogData.h"
#include "EdgeBaseDefine.h"

CConfig::CConfig()
    :max_log_file_num_(5),
    max_log_line_num_(1000),
    log_level_(3),
    super_node_port_(0),
    ping_interval_(120)
{
}

CConfig::~CConfig()
{
}


void CConfig::Load(const std::string& file, bool fully_loaded)
{
    char buff[1024];

    memset(buff, 0, sizeof(buff));
    GetPrivateProfileString(NAME_CONFIG_SECTION_LOG, NAME_CONFIG_KEY_LOG_DIR, "", buff, 1023, file.c_str());
    log_dir_ = buff;

    memset(buff, 0, sizeof(buff));
    GetPrivateProfileString(NAME_CONFIG_SECTION_LOG, NAME_CONFIG_KEY_LOG_NAME, "CMEdge", buff, 1023, file.c_str());
    log_file_ = buff;

    max_log_file_num_ = GetPrivateProfileInt(NAME_CONFIG_SECTION_LOG, NAME_CONFIG_KEY_LOG_FILENUM, 5, file.c_str());

    max_log_line_num_ = GetPrivateProfileInt(NAME_CONFIG_SECTION_LOG, NAME_CONFIG_KEY_LOG_LINENUM, 1000, file.c_str());

    log_level_ = GetPrivateProfileInt(NAME_CONFIG_SECTION_LOG, NAME_CONFIG_KEY_LOG_LEVEL, 3, file.c_str());

    if (false == fully_loaded)
    {
        return;
    }

    memset(buff, 0, sizeof(buff));
    GetPrivateProfileString(NAME_CONFIG_SECTION_SUPERNODE, NAME_CONFIG_KEY_SUPERNODE_IP, "", buff, 1023, file.c_str());
    super_node_ip_ = buff;

    super_node_port_ = GetPrivateProfileInt(NAME_CONFIG_SECTION_SUPERNODE, NAME_CONFIG_KEY_SUPERNODE_PORT, 0, file.c_str());

    memset(buff, 0, sizeof(buff));
    GetPrivateProfileString(NAME_CONFIG_SECTION_EDGE, NAME_CONFIG_KEY_EDGE_IP, "", buff, 1023, file.c_str());
    edge_ip_ = buff;

    memset(buff, 0, sizeof(buff));
    GetPrivateProfileString(NAME_CONFIG_SECTION_EDGE, NAME_CONFIG_KEY_EDGE_INTERFACE, "", buff, 1023, file.c_str());
    interface_name_ = buff;

    memset(buff, 0, sizeof(buff));
    GetPrivateProfileString(NAME_CONFIG_SECTION_EDGE, NAME_CONFIG_KEY_EDGE_GROUPNAME, "", buff, 1023, file.c_str());
    group_name_ = buff;

    memset(buff, 0, sizeof(buff));
    GetPrivateProfileString(NAME_CONFIG_SECTION_EDGE, NAME_CONFIG_KEY_EDGE_PASSWORD, "", buff, 1023, file.c_str());
    password_ = buff;

    memset(buff, 0, sizeof(buff));
    GetPrivateProfileString(NAME_CONFIG_SECTION_EDGE, NAME_CONFIG_KEY_EDGE_MAC_ADDR, "", buff, 1023, file.c_str());
    mac_address_ = buff;

    memset(buff, 0, sizeof(buff));
    GetPrivateProfileString(NAME_CONFIG_SECTION_EDGE, NAME_CONFIG_KEY_EDGE_MASK, "255.255.255.0", buff, 1023, file.c_str());
    mask_ = buff;

    mtu_ = GetPrivateProfileInt(NAME_CONFIG_SECTION_EDGE, NAME_CONFIG_KEY_EDGE_MTU, 1400, file.c_str());

    ping_interval_ = GetPrivateProfileInt(NAME_CONFIG_SECTION_PING, NAME_CONFIG_KEY_PING_INTERVAL, 120, file.c_str());

    memset(buff, 0, sizeof(buff));
    GetPrivateProfileString(NAME_CONFIG_SECTION_PING, NAME_CONFIG_KEY_PING_DSTADDR, "", buff, 1023, file.c_str());
    ping_dst_address_ = buff;

    memset(buff, 0, sizeof(buff));
    GetPrivateProfileString(NAME_CONFIG_SECTION_VNC, NAME_CONFIG_KEY_VNC_PATH, "", buff, 1023, file.c_str());
    vnc_path_ = buff;
}

const std::string& CConfig::GetLogDir() const
{
    return log_dir_;
}

const std::string& CConfig::GetLogFile() const
{
    return log_file_;
}

size_t CConfig::GetMaxLogFileNum() const
{
    return max_log_file_num_;
}

size_t CConfig::GetMaxLogLineNum() const
{
    return max_log_line_num_;
}

long CConfig::GetLogLevel() const
{
    return log_level_;
}

const std::string& CConfig::GetSuperNodeIP() const
{
    return super_node_ip_;
}

USHORT CConfig::GetSuperNodePort() const
{
    return super_node_port_;
}

const std::string& CConfig::GetInterfaceName() const
{
    return interface_name_;
}

const std::string& CConfig::GetEdgeIP() const
{
    return edge_ip_;
}

const std::string& CConfig::GetGroupName() const
{
    return group_name_;
}

const std::string& CConfig::GetPassword() const
{
    return password_;
}

const std::string& CConfig::GetMacAddress() const
{
    return mac_address_;
}

const std::string& CConfig::GetMask() const
{
    return mask_;
}

const int CConfig::GetMTU() const
{
    return mtu_;
}

UINT CConfig::GetPingInterval() const
{
    return ping_interval_;
}

const std::string& CConfig::GetPingDstAddress() const
{
    return ping_dst_address_;
}

const std::string& CConfig::GetVncPath() const
{
    return vnc_path_;
}
