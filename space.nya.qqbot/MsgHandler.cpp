
#include "MsgHandler.h"
#include "Helper.h"
#include "base64.h"
#include<regex>
MsgHandler::MsgHandler(int32_t _ac)
{
	using namespace std;
	ac = _ac;
	ignoreList.insert(std::make_pair(1119116149, 1));
	adminList.insert(std::make_pair(147460375, 1));
	stime = Helper::getDate();

	logFile.open("log/LOG_" + stime + ".txt", ios::in);
	int32_t type, subType, msgId, font;
	int64_t fromGroup, fromQQ;
	string msg;
	while (logFile >> type >> subType >> msgId >> fromGroup >> fromQQ >> msg >> font) {
		
		msg = base64_decode(msg);

		recoverMsg(type, subType, msgId, fromGroup, fromQQ, msg, font);
	}
	logFile.close();
	//CQ_addLog(ac, CQLOG_DEBUG, "debug", stime.c_str());
	logFile.open("log/LOG_" + stime + ".txt",ios::out|ios::app);
}

void MsgHandler::NewMsg(int32_t type, int32_t subType, int32_t msgId,
	int64_t fromGroup, int64_t fromQQ, const std::string& msg, int32_t font) {
	using namespace std;
	auto b64_msg = base64_encode((const unsigned char *)msg.c_str(), msg.size());
	//CQ_addLog(ac, CQLOG_DEBUG, "base64", b64_msg.c_str());
	logFile << type << " " << subType << " " << msgId << " " << fromGroup << " " << fromQQ << " " << b64_msg << " " << font << endl << flush;
	regex history(R"(!history.*)");
	if (ignoreList.count(fromQQ) == 0) {
		if (regex_match(msg,history)) {
			regex historyPrt(R"(\[CQ:at,qq=([0-9]+)\])");
			smatch results;
			string str;

			if (std::regex_search(msg, results, historyPrt)) {

				int64_t qq=0;
				for (auto i : results) {
					if (regex_match(i.str(), regex(R"([0-9]+)"))) {
						qq = _atoi64(i.str().c_str());
					}					
				}
				if (qq != 0){
					str = getUserMsg(fromGroup, qq);
				}
			}
			else {
				str = getUserMsg(fromGroup, fromQQ);
			}
			
			CQ_sendGroupMsg(ac, fromGroup, str.c_str());
		}
	}
	insertUserMsg(fromGroup, fromQQ, msg);
	
}
void MsgHandler::recoverMsg(int32_t type, int32_t subType, int32_t msgId,
	int64_t fromGroup, int64_t fromQQ, const std::string& msg, int32_t font) {
	using namespace std;
	insertUserMsg(fromGroup, fromQQ, msg);
}

void MsgHandler::insertUserMsg(int64_t fromGroup, int64_t fromQQ, const std::string& msg) {
	if (userMsgs.count(fromQQ) == 0) {
		userMsgs.insert(
			std::pair < 
				int64_t, 
				std::map<std::int64_t, std::deque<std::string>>>(fromQQ, std::map < std::int64_t, std::deque<std::string>>()));

	}
	if (userMsgs.find(fromQQ)->second.count(fromGroup) == 0) {
		userMsgs.find(fromQQ)->second.insert(
			std::pair <int64_t,std::deque < std::string >> (
				fromGroup, std::deque<std::string>())
			);
	}
	if (userMsgs[fromQQ][fromGroup].size() == userMsgLimit) {
		userMsgs[fromQQ][fromGroup].pop_front();
		userMsgs[fromQQ][fromGroup].push_back(msg);
	}
	else {
		userMsgs[fromQQ][fromGroup].push_back(msg);
	}
}
std::string MsgHandler::getUserMsg(int64_t fromGroup, int64_t fromQQ){
	std::string ret="";
	if (userMsgs.count(fromQQ) == 0) {
		return ret;
	}
	if (userMsgs.find(fromQQ)->second.count(fromGroup) == 0) {
		return ret;
	}
	for(auto i:userMsgs[fromQQ][fromGroup]){
		ret += i+"\n";
	}
	return ret;
}

MsgHandler::~MsgHandler()
{
	logFile.close();
}
