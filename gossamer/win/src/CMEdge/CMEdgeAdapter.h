
#ifndef _CM_EDGE_ADAPTER_H_
#define _CM_EDGE_ADAPTER_H_

#include "utils\ThreadService.h"
#include <atlbase.h>
#include <atlstr.h>
#include <string>

class CCMEdgeAdapter : public CThreadService
{
public:
    CCMEdgeAdapter();
    virtual ~CCMEdgeAdapter();

    int Start();
    int Stop();

    void SetSuperNodeIP(const std::string& ip);
    void SetInterfaceName(const std::string& name);
    void SetEdgeIP(const std::string& ip);
    void SetGroupName(const std::string& name);
    void SetPassword(const std::string& password);
    void SetMacAddress(const std::string& address);
    void SetMask(const std::string& mask);
    void SetMTU(int mtu);

protected:
    virtual int Scheduler(CThreadExcuteData* thread_data);

private:
    std::string super_node_ip_;
    std::string interface_name_;
    std::string edge_ip_;
    std::string group_name_;
    std::string password_;
    std::string mac_address_;
    std::string mask_;
    int mtu_;
};


#endif // !_CM_EDGE_ADAPTER_H_
