#ifdef WIN32
#include "stdafx.h"
#endif

#define ONFRONTCONNECTED 0
#define ONFRONTDISCONNECTED 1
#define ONHEARTBEATWARNING 2
#define ONRSPUSERLOGIN 3
#define ONRSPUSERLOGOUT 4
#define ONRSPERROR 5
#define ONRSPSUBMARKETDATA 6
#define ONRSPUNSUBMARKETDATA 7
#define ONRSPSUBFORQUOTERSP 8
#define ONRSPUNSUBFORQUOTERSP 9
#define ONRTNDEPTHMARKETDATA 10
#define ONRTNFORQUOTERSP 11


#include "sopt/ThostFtdcMdApi.h"
#include "pybind11/pybind11.h"
#include "vnsopt.h"
using namespace pybind11;

class MdApi :public CThostFtdcMdSpi
{
private:
	CThostFtdcMdApi* api;
	TaskQueue task_queue;
	bool active = false;
	thread task_thread;
public:
	MdApi() {};
	~MdApi() {
		if (this->active) {
			this->exit();
		}
	}

	/// <summary>
	/// 这里是spi的回调函数
	/// </summary>
		///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected() ;

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	virtual void OnFrontDisconnected(int nReason) ;

	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	///@param nTimeLapse 距离上次接收报文的时间
	virtual void OnHeartBeatWarning(int nTimeLapse) ;


	///登录请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) ;

	///登出请求响应
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField* pUserLogout, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) ;

	///错误应答
	virtual void OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) ;

	///订阅行情应答
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) ;

	///取消订阅行情应答
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) ;

	///订阅询价应答
	virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) ;

	///取消订阅询价应答
	virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) ;

	///深度行情通知
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData) ;

	///询价通知
	virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField* pForQuoteRsp) ;

	////////////////////////////////////////////////////////////
	//////////这里是python的回调函数，全部小写开头
	////////////////////////////////////////////////////////////

	virtual void onFrontConnected() {};

	virtual void onFrontDisconnected(int reqid) {};

	virtual void onHeartBeatWarning(int reqid) {};

	virtual void onRspUserLogin(const dict& data, const dict& error, int reqid, bool last) {};

	virtual void onRspUserLogout(const dict& data, const dict& error, int reqid, bool last) {};

	virtual void onRspError(const dict& error, int reqid, bool last) {};

	virtual void onRspSubMarketData(const dict& data, const dict& error, int reqid, bool last) {};

	virtual void onRspUnSubMarketData(const dict& data, const dict& error, int reqid, bool last) {};

	virtual void onRspSubForQuoteRsp(const dict& data, const dict& error, int reqid, bool last) {};

	virtual void onRspUnSubForQuoteRsp(const dict& data, const dict& error, int reqid, bool last) {};

	virtual void onRtnDepthMarketData(const dict& data) {};

	virtual void onRtnForQuoteRsp(const dict& data) {};




	//////////////////////////////////////////////////
	/////process
	//////////////////////////////////////////////////
	void processTask();
	
	void processFrontConnected(Task* task);

	void processFrontDisconnected(Task* task);

	void processHeartBeatWarning(Task* task);

	void processRspUserLogin(Task* task);

	void processRspUserLogout(Task* task);

	void processRspError(Task* task);

	void processRspSubMarketData(Task* task);

	void processRspUnSubMarketData(Task* task);

	void processRspSubForQuoteRsp(Task* task);

	void processRspUnSubForQuoteRsp(Task* task);

	void processRtnDepthMarketData(Task* task);

	void processRtnForQuoteRsp(Task* task);




	//////////////////////////////////////////////////
	/////api  主动函数
	//////////////////////////////////////////////////
	void createFtdcMdApi(string pszFlowPath = "");

	void release();

	void init();

	int join();

	int exit();

	string getTradingDay();
	string getApiVersion();

	void registerFront(string pszFrontAddress);

	int subscribeMarketData(string instrumentID);

	int unSubscribeMarketData(string instrumentID);

	int subscribeForQuoteRsp(string instrumentID);

	int unSubscribeForQuoteRsp(string instrumentID);

	int reqUserLogin(const dict &req, int reqid);

	int reqUserLogout(const dict &req, int reqid);
};