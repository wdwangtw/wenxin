#include "stdafx.h"
#include "CMEdgeAdapter.h"
#include "edge.h"

CCMEdgeAdapter::CCMEdgeAdapter()
    :mtu_(1400),
    mask_("255.255.255.0")
{
}

CCMEdgeAdapter::~CCMEdgeAdapter()
{
    Stop();
}

int CCMEdgeAdapter::Start()
{
    if (0 == OpenThread())
    {
        return -1;
    }

    return 0;
}

int CCMEdgeAdapter::Stop()
{
    CloseThread();

    return 0;
}

int CCMEdgeAdapter::Scheduler(CThreadExcuteData* thread_data)
{
    int     local_port = 0 /* any port */;
    int     mgmt_port = 5644; /* 5644 by default */
    char    tuntap_dev_name[N2N_IFNAMSIZ] = "edge0";
    char    ip_mode[16] = "static";
    char    ip_addr[16] = "";
    char    netmask[16] = "255.255.255.0";
    int     mtu = mtu_;
    char    device_mac[18] = "";
    std::string encrypt_key = password_;

    if (mask_.length())
    {
        strcpy(netmask, mask_.c_str());
    }

    strcpy(device_mac, mac_address_.c_str());
    strcpy(tuntap_dev_name, interface_name_.c_str());
    strcpy(ip_addr, edge_ip_.c_str());

    n2n_edge_t eee; /* single instance for this program */

    if (-1 == edge_init(&eee))
    {
        //traceEvent(TRACE_ERROR, "Failed in edge_init");
        return -1;
    }

    eee.allow_routing = 1;

    eee.re_resolve_supernode_ip = 1;        // -b

    memset(eee.community_name, 0, N2N_COMMUNITY_SIZE);
    strcpy(reinterpret_cast<char*>(eee.community_name), group_name_.c_str());

    memset(&(eee.supernode), 0, sizeof(eee.supernode));
    eee.supernode.family = AF_INET;

    eee.sn_num = 1;
    strcpy(eee.sn_ip_array[0], super_node_ip_.c_str());

    //traceEvent(TRACE_NORMAL, "Starting n2n edge %s %s", n2n_sw_version, n2n_sw_buildDate);

    //for (int i = 0; i < N2N_EDGE_NUM_SUPERNODES; ++i)
    //{
    //    //traceEvent(TRACE_NORMAL, "supernode %u => %s\n", i, (eee.sn_ip_array[i]));
    //}

    supernode2addr(&(eee.supernode), eee.sn_ip_array[eee.sn_idx]);

    if (0 == encrypt_key.length())
    {
        //traceEvent(TRACE_WARNING, "Encryption is disabled in edge.");

        eee.null_transop = 1;
    }

    if (0 == strcmp("dhcp", ip_mode))
    {
        //traceEvent(TRACE_NORMAL, "Dynamic IP address assignment enabled.");
        eee.dyn_ip_mode = 1;
    }
    else
    {
        //traceEvent(TRACE_NORMAL, "ip_mode='%s'", ip_mode);
    }

    if (tuntap_open(&(eee.device), tuntap_dev_name, ip_mode, ip_addr, netmask, device_mac, mtu) < 0)
    {
        return -1;
    }

    if (local_port > 0)
    {
        //traceEvent(TRACE_NORMAL, "Binding to local port %d", (signed int)local_port);
    }

    if (encrypt_key.length())
    {
        if (edge_init_twofish(&eee, (uint8_t *)(encrypt_key.c_str()), encrypt_key.length()) < 0)
        {
            fprintf(stderr, "Error: twofish setup failed.\n");
            return -1;
        }
    }

    eee.udp_sock = open_socket(local_port, 1 /*bind ANY*/);
    if (eee.udp_sock < 0)
    {
        //traceEvent(TRACE_ERROR, "Failed to bind main UDP port %u", (signed int)local_port);
        return -1;
    }

    eee.udp_mgmt_sock = open_socket(mgmt_port, 0 /* bind LOOPBACK*/);

    if (eee.udp_mgmt_sock < 0)
    {
        return -1;
    }

    //traceEvent(TRACE_NORMAL, "edge started");

    update_supernode_reg(&eee, time(NULL));

    run_loop(&eee);

    return -1;
}

void CCMEdgeAdapter::SetSuperNodeIP(const std::string& ip)
{
    super_node_ip_ = ip;
}

void CCMEdgeAdapter::SetInterfaceName(const std::string& name)
{
    interface_name_ = name;
}

void CCMEdgeAdapter::SetEdgeIP(const std::string& ip)
{
    edge_ip_ = ip;
}

void CCMEdgeAdapter::SetGroupName(const std::string& name)
{
    group_name_ = name;
}

void CCMEdgeAdapter::SetPassword(const std::string& password)
{
    password_ = password;
}

void CCMEdgeAdapter::SetMacAddress(const std::string& address)
{
    mac_address_ = address;
}

void CCMEdgeAdapter::SetMask(const std::string& mask)
{
    if (mask.length() > 15)
    {
        return;
    }

    mask_ = mask;
}

void CCMEdgeAdapter::SetMTU(int mtu)
{
    mtu_ = mtu;
}
