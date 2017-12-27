#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <windows.h>

class CConfig
{
public:
    CConfig();
    ~CConfig();

    void Load(const std::string& file, bool fully_loaded = true);

    const std::string& GetLogDir() const;
    const std::string& GetLogFile() const;
    size_t GetMaxLogFileNum() const;
    size_t GetMaxLogLineNum() const;
    long GetLogLevel() const;

    const std::string& GetSuperNodeIP() const;
    USHORT GetSuperNodePort() const;
    const std::string& GetInterfaceName() const;
    const std::string& GetEdgeIP() const;
    const std::string& GetGroupName() const;
    const std::string& GetPassword() const;
    const std::string& GetMacAddress() const;
    const std::string& GetMask() const;
    const int GetMTU() const;

    UINT GetPingInterval() const;
    const std::string& GetPingDstAddress() const;

    const std::string& GetVncPath() const;

private:
    std::string log_dir_;
    std::string log_file_;
    size_t max_log_file_num_;
    size_t max_log_line_num_;
    long log_level_;

    std::string super_node_ip_;
    USHORT super_node_port_;

    std::string interface_name_;
    std::string edge_ip_;
    std::string group_name_;
    std::string password_;
    std::string mac_address_;
    std::string mask_;
    int mtu_;

    std::string vnc_path_;

    UINT ping_interval_;
    std::string ping_dst_address_;
};

#endif // !_CONFIG_H_
