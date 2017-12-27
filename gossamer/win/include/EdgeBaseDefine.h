
#ifndef _EDGE_BASE_DEFINE_H_
#define _EDGE_BASE_DEFINE_H_

/********************************************************/
// ��������
#define NAME_SERVICE                    "gossamer"
// �˳��¼�����
#define NAME_EVENT_SERVICEEXIT          "3D175150-A09D-4F3C-9759-ECB9B5726C18"
// �����ļ�����
#define NAME_CONFIG_FILE                "gossamer.ini"
// �����ļ���Section����
#define NAME_CONFIG_SECTION_LOG         "log"
#define NAME_CONFIG_SECTION_SUPERNODE   "server"
#define NAME_CONFIG_SECTION_EDGE        "client"
#define NAME_CONFIG_SECTION_PING        "live"
#define NAME_CONFIG_SECTION_VNC         "vnc"
// �����ļ���Key����
#define NAME_CONFIG_KEY_LOG_DIR         "path"
#define NAME_CONFIG_KEY_LOG_NAME        "filename"
#define NAME_CONFIG_KEY_LOG_FILENUM     "filenum"
#define NAME_CONFIG_KEY_LOG_LINENUM     "linenum"
#define NAME_CONFIG_KEY_LOG_LEVEL       "level"
#define NAME_CONFIG_KEY_SUPERNODE_IP    "ip"
#define NAME_CONFIG_KEY_SUPERNODE_PORT  "port"
#define NAME_CONFIG_KEY_EDGE_IP         "ip"
#define NAME_CONFIG_KEY_EDGE_INTERFACE  "interface"
#define NAME_CONFIG_KEY_EDGE_GROUPNAME  "groupname"
#define NAME_CONFIG_KEY_EDGE_PASSWORD   "password"
#define NAME_CONFIG_KEY_EDGE_MAC_ADDR   "mac"
#define NAME_CONFIG_KEY_EDGE_MTU        "mtu"
#define NAME_CONFIG_KEY_EDGE_MASK       "mask"
#define NAME_CONFIG_KEY_PING_INTERVAL   "interval"
#define NAME_CONFIG_KEY_PING_DSTADDR    "dst"
#define NAME_CONFIG_KEY_VNC_PATH        "path"
// �ؼ���
#define KEYWORD_INSTALL                 " -install"
#define KEYWORD_UNINSTALL               " -uninstall"

/********************************************************/
// gSOAP URL �ֶ�
#define GSOAP_FIELD_VNCSTARTUPPARAM     "startupparam"
#define GSOAP_FIELD_CALLBACK            "callback"
#define GSOAP_JSON_FIELD_RESULT         "result"
#define GSOAP_JSON_FIELD_VERSION        "version"

// ����������
#define NAME_PROCESSOR_STARTUPVNC       "startupvnc"
#define NAME_PROCESSOR_GETVERSION       "getversion"

// ������
enum EErrorCode
{
    EC_Succeed = 0,
    EC_InvalidParams = 1000,
    EC_InvalidVNC,
};

#endif //!_EDGE_BASE_DEFINE_H_
