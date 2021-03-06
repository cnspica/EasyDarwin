/*
	Copyright (c) 2012-2016 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.easydarwin.org
*/
#ifndef EASY_PROTOCOL_H
#define	EASY_PROTOCOL_H

#include <EasyProtocolBase.h>
#include <map>
#include <vector>
#include <boost/variant.hpp>
#include <boost/lexical_cast.hpp>
#include <list>
#include <set>
using namespace std;

namespace EasyDarwin { namespace Protocol
{

class Easy_API EasyDevice//摄像头信息类
{
public:
	EasyDevice();
	EasyDevice(const string& serial, const string& name);
	EasyDevice(const string& serial, const string& name, const string& status);
	~EasyDevice(){}

public:
	string serial_;
	string name_;
	string status_;	//online/offline
	string password_;
	string tag_;
	string channel_;
	string snapJpgPath_;//最新的快照路径
};

//typedef vector<EasyDevice> EasyDevices;		//摄像头数组
//typedef EasyDevices::iterator EasyDevicesIterator;

typedef map<string,EasyDevice> EasyDevices;		//摄像头表，改为map.方便查找。key为channel_，value为EasyDevice
typedef EasyDevices::iterator EasyDevicesIterator;


typedef boost::variant<int, float, string> value_t;
typedef map<string, value_t> EasyJsonValue;	//key为string,value可以是int、float、string的一种
typedef void* EasyObject;


typedef struct
{
	string device_serial_;
	string camera_serial_;
	string stream_id_;
	EasyObject object_;
	unsigned long timestamp_;
	unsigned long timeout_;//second
	int message_type_;//see EasyDarwin_Message_Type_Define
} EasyNVRMessage;//NVR消息

typedef vector<EasyNVRMessage> EasyNVRMessageQueue;//NVR消息数组


class EasyJsonValueVisitor : public boost::static_visitor<string>
{
public:
	string operator()(int value) const { return boost::lexical_cast<string>(value); }
	string operator()(float value) const { return boost::lexical_cast<string>(value); }
	string operator()(string value) const { return value; }
};

class Easy_API EasyNVR : public EasyDevice//NVR类型
{
public:
	EasyNVR();
	EasyNVR(const string& serial, const string& name, const string& password, const string& tag, EasyDevices &channel);
	~EasyNVR(){}

public:	
	EasyDevices channels_;                                                           
	EasyObject object_;
};

// MSG_DS_REGISTER_REQ
class Easy_API EasyMsgDSRegisterREQ : public EasyProtocol	//封装NVR的注册请求
{
public:
	EasyMsgDSRegisterREQ(EasyDarwinTerminalType terminalType, EasyDarwinAppType appType, EasyNVR &nvr, size_t cseq = 1);
	EasyMsgDSRegisterREQ(const char* msg);
	virtual ~EasyMsgDSRegisterREQ() {}

public:
	EasyNVR& GetNVR() { return nvr_; }

private:
	EasyNVR nvr_;
};

// MSG_SD_REGISTER_ACK
class Easy_API EasyMsgSDRegisterACK : public EasyProtocol	//封装NVR的注册回应
{
public:
	EasyMsgSDRegisterACK(EasyJsonValue &body, size_t cseq = 1, size_t error = 200);
	EasyMsgSDRegisterACK(const char* msg);
	virtual ~EasyMsgSDRegisterACK() {}
};

// MSG_SD_PUSH_STREAM_REQ
class EasyMsgSDPushStreamREQ : public EasyProtocol
{
public:
	EasyMsgSDPushStreamREQ(EasyJsonValue &body, size_t cseq);
	EasyMsgSDPushStreamREQ(const char* msg);
	virtual ~EasyMsgSDPushStreamREQ() {}
};

// MSG_DS_PUSH_STREAM_ACK
class EasyMsgDSPushSteamACK : public EasyProtocol
{
public:
	EasyMsgDSPushSteamACK(EasyJsonValue &body, size_t cseq = 1, size_t error = 200);
	EasyMsgDSPushSteamACK(const char* msg);
	virtual ~EasyMsgDSPushSteamACK() {}
};

// MSG_SD_STREAM_STOP_REQ
class EasyMsgSDStopStreamREQ : public EasyProtocol
{
public:
	EasyMsgSDStopStreamREQ(EasyJsonValue &body, size_t cseq = 1);
	EasyMsgSDStopStreamREQ(const char* msg);
	virtual ~EasyMsgSDStopStreamREQ() {}
};

// MSG_DS_STREAM_STOP_ACK
class EasyMsgDSStopStreamACK : public EasyProtocol
{
public:
	EasyMsgDSStopStreamACK(EasyJsonValue &body, size_t cseq = 1, size_t error = 200);
	EasyMsgDSStopStreamACK(const char* msg);
	virtual ~EasyMsgDSStopStreamACK() {}
};

// MSG_SC_DEVICE_LIST_ACK
class EasyMsgSCDeviceListACK : public EasyProtocol
{
public:
	EasyMsgSCDeviceListACK(EasyDevices &devices, size_t cseq = 1, size_t error = 200);
	EasyMsgSCDeviceListACK(const char* msg);
	virtual ~EasyMsgSCDeviceListACK() {}

	EasyDevices& GetDevices() { return devices_; }

private:
	EasyDevices devices_;
};

// MSG_SC_DEVICE_INFO_ACK
class EasyMsgSCDeviceInfoACK : public EasyProtocol
{
public:
	EasyMsgSCDeviceInfoACK(EasyDevices &cameras, const string& devcei_serial, size_t cseq = 1, size_t error = 200);
	EasyMsgSCDeviceInfoACK(const char* msg);
	~EasyMsgSCDeviceInfoACK() {}

	EasyDevices& GetCameras() { return channels_; }

private:
	EasyDevices channels_;
};

// MSG_SC_GET_STREAM_ACK
class EasyMsgSCGetStreamACK : public EasyProtocol
{
public:
	EasyMsgSCGetStreamACK(EasyJsonValue &body, size_t cseq = 1, size_t error = 200);
	EasyMsgSCGetStreamACK(const char* msg);
	virtual ~EasyMsgSCGetStreamACK() {}
};

// MSG_CS_FREE_STREAM_REQ
class EasyMsgCSFreeStreamREQ : public EasyProtocol
{
public:
	EasyMsgCSFreeStreamREQ(EasyJsonValue &body, size_t cseq);
	EasyMsgCSFreeStreamREQ(const char* msg);
	virtual ~EasyMsgCSFreeStreamREQ() {}
};

// MSG_SC_FREE_STREAM_ACK
class EasyMsgSCFreeStreamACK : public EasyProtocol
{
public:
	EasyMsgSCFreeStreamACK(EasyJsonValue &body, size_t cseq = 1, size_t error = 200);
	EasyMsgSCFreeStreamACK(const char* msg);
	virtual ~EasyMsgSCFreeStreamACK() {}
};

// MSG_DS_POST_SNAP_REQ
class EasyMsgDSPostSnapREQ : public EasyProtocol
{
public:
	EasyMsgDSPostSnapREQ(EasyJsonValue &body, size_t cseq = 1);
	EasyMsgDSPostSnapREQ(const char* msg);
	virtual ~EasyMsgDSPostSnapREQ() {}
};

// MSG_SD_POST_SNAP_ACK
class EasyMsgSDPostSnapACK : public EasyProtocol
{
public:
	EasyMsgSDPostSnapACK(EasyJsonValue &body, size_t cseq = 1, size_t error = 200);
	EasyMsgSDPostSnapACK(const char* msg);
	virtual ~EasyMsgSDPostSnapACK() {}
};

class EasyMsgExceptionACK : public EasyProtocol
{
public:
	EasyMsgExceptionACK(size_t cseq = 1, size_t error = 400);
	virtual ~EasyMsgExceptionACK() {}
};

class EasyDarwinHLSession
{
public:
	EasyDarwinHLSession(){}
	~EasyDarwinHLSession(){}

public:
	int index;
	std::string SessionName;
	std::string HlsUrl;
	std::string sourceUrl;
	int bitrate;
};

class EasyDarwinRTSPSession
{
public:
	EasyDarwinRTSPSession(){}
	~EasyDarwinRTSPSession(){}

public:
	int index;
	std::string Url;
	std::string Name;
	int numOutputs;
};

// MSG_SC_START_HLS_ACK
class Easy_API EasyMsgSCStartHLSACK : public EasyProtocol
{
public:
	EasyMsgSCStartHLSACK();
	EasyMsgSCStartHLSACK(const char *msg);
	virtual ~EasyMsgSCStartHLSACK() {}

	void SetStreamName(const char* sName);
	void SetStreamURL(const char* sURL);
};

// MSG_SC_HLS_SESSION_LIST_ACK
class Easy_API EasyMsgSCHLSessionListACK : public EasyProtocol
{
public:
	EasyMsgSCHLSessionListACK();
	EasyMsgSCHLSessionListACK(const char* msg);
	virtual ~EasyMsgSCHLSessionListACK() {}

public:
	bool AddSession(EasyDarwinHLSession &session);
	//int StartGetDevice();
	//bool GetNextDevice(EasyDarwinDevice &device);

private:
	list<EasyDarwinHLSession> sessions;
};

// MSG_SC_RTSP_PUSH_SESSION_LIST_ACK
class Easy_API EasyMsgSCRTSPPushSessionListACK : public EasyProtocol
{
public:
	EasyMsgSCRTSPPushSessionListACK();
	EasyMsgSCRTSPPushSessionListACK(const char* msg);
	virtual ~EasyMsgSCRTSPPushSessionListACK() {}

public:
	bool AddSession(EasyDarwinRTSPSession &session);
	//int StartGetDevice();
	//bool GetNextDevice(EasyDarwinDevice &device);

private:
	std::list<EasyDarwinRTSPSession> sessions;
};

// MSG_SC_LIST_RECORD_ACK
class Easy_API EasyMsgSCListRecordACK : public EasyProtocol
{
public:
	EasyMsgSCListRecordACK();
	EasyMsgSCListRecordACK(const char* msg);
	virtual ~EasyMsgSCListRecordACK() {}

public:
	bool AddRecord(const string& record);

private:
	std::list<std::string> records;
};

class Easy_API EasyProtocolACK : public EasyProtocol
{
public:
	EasyProtocolACK(int iMsgType) :EasyProtocol(iMsgType) {}
	void SetHead(EasyJsonValue& header);//设置头部
	void SetBody(EasyJsonValue& body);//设置JSON
};

class Easy_API EasyMsgSCRecordListACK : public EasyProtocolACK//封装录像列表回应
{
public:
	EasyMsgSCRecordListACK(int iMsgType) :EasyProtocolACK(iMsgType) {}
	void AddRecord(const string& record);
};

//add,Unisiot，start
typedef struct 
{
	string strDeviceSerial;//设备序列号
	string strCameraSerial;//摄像头序列号
	string strProtocol;//直播协议
	string strStreamID;//直播流类型
} stStreamInfo;

class strDevice//设备类型对应的信息
{
public:
	strDevice();
	bool GetDevInfo(const char *msg);//由JSON文本得到设备信息
	void HoldSnapPath(const string& strJpgPath, const string& strChannel);//保留快照的路径
public:
	string serial_;//设备序列号
	string name_;//设备名称
	string password_;//密码
	string tag_;//标签
	string channelCount_;//该设备包含的摄像头个数
	string snapJpgPath_;//最新的快照路径


	EasyDevices channels_;//摄像头信息
	EasyDarwinTerminalType eDeviceType;//设备类型
	EasyDarwinAppType eAppType;//App类型
};
//add,Unisiot，end
}}//namespace
#endif
