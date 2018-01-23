#pragma once
#include "stdafx.h"
#include<iostream>
#include<fstream>
#include<time.h>
#include<string>
#include<thread>
#include"cqp.h"
#include"json\json.h"

class MsgHandler
{
protected:
	int userMsgLimit = 6;
	std::fstream logFile;
	std::string stime;
	int32_t ac;
	//map(qq,osuid)
	std::map<int64_t, std::string> ids;
	//map(qq,(group,msgs))
	std::map<int64_t, std::map<std::int64_t,std::deque<std::string>>> userMsgs;
	
	std::map<int64_t, int32_t> ignoreList;
	std::map<int64_t, int32_t> adminList;

public:
	
	MsgHandler(int32_t);
	void NewMsg(int32_t type, int32_t subType,int32_t msgId,
		int64_t fromGroup, int64_t fromQQ,const std::string& msg, int32_t font);
	void recoverMsg(int32_t type, int32_t subType, int32_t msgId,
		int64_t fromGroup, int64_t fromQQ,const std::string& msg, int32_t font);
	void insertUserMsg(int64_t fromGroup, int64_t fromQQ, const std::string& msg);
	std::string getUserMsg(int64_t fromGroup, int64_t fromQQ);
	~MsgHandler();
};

