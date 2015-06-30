// TestAppRealKit.cpp : Defines the entry point for the console application.
//

#pragma comment(lib, "LibAppRealKit.lib")
#include <string>
#include <iostream>
#include <limits>
#include <functional>
#include "stdafx.h"
#include "include/constant.h"
#include "include/balancer.h"

void waitOnce()
{
	std::string s;
	std::getline(std::cin, s);
}

void openCallback(const std::string &sMessage, int iCode) {
	std::cout << "openCallback " << sMessage << " with code=" << iCode << std::endl;
}

void messageCallback(const std::string &sMessage, int iCode) {
	std::cout << "messageCallback " << sMessage << " with code=" << iCode << std::endl;
}

void closeCallback(const std::string &sMessage, int iCode) {
	std::cout << "closeCallback " << sMessage << " with code=" << iCode << std::endl;
}

void autheticateCallback(const std::string &sMessage, int iCode) {
	std::cout << "autheticateCallback " << sMessage << " with code=" << iCode << std::endl;
}

void createRoomCallback(const std::string &sMessage, int iCode) {
	std::cout << "createRoomCallback " << sMessage << " with code=" << iCode << std::endl;
}

void getLoginCallback(const std::string &sMessage, int iCode) {
	std::cout << "getLoginCallback " << sMessage << " with code=" << iCode << std::endl;
}

void notifyUsersCallback(const std::string &sMessage, int iCode) {
	std::cout << "notifyUsersCallback " << sMessage << " with code=" << iCode << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::string appID_="080693ab-7123-4aee-b560-9f6a8a332a43";
	std::string appSecret_="VORAI5D6yz";
	std::string hostIP_="192.168.10.102";
	int iPort_=7777;
	std::string loginID_ = "1234";
	std::string memberID_ = "5678";

	//Get the balancer instance
	AppRealKit::Realtime::Balancer *oBalancer = AppRealKit::Realtime::Balancer::getInstance();

	//Set debug FLG
	oBalancer->setVerbose(false);

	//Set the product ID and Secret
	oBalancer->setProductID(appID_, appSecret_);
	
	//Set the callback
	oBalancer->onKitOpen(openCallback);
	oBalancer->onKitMessage(messageCallback);
	oBalancer->onKitClose(closeCallback);

	//Connect to HOST
	oBalancer->connect(hostIP_, iPort_);

	//Wait to open the socket
	std::cout << "Press ENTER to authenticate" << std::endl;
	char enterKey;
	std::cin.get(enterKey);

	//Call AUTHENTICATE method
	oBalancer->authenticate(loginID_, autheticateCallback);

	//Wait to open the socket
	std::cout << "Press ENTER to create ROOM" << std::endl;	
	std::cin.get(enterKey);

	//Call create ROOM
	std::map<std::string, std::string> properties;
	properties.insert(std::pair<std::string, std::string>("name_window", "ROOM DEMO from WINDOW"));
	properties.insert(std::pair<std::string, std::string>("description", "ROOM DESCRIPTION"));
	int iCapacity = 0; //Unlimited members in a ROOM
	oBalancer->createRoom(loginID_, properties, iCapacity, createRoomCallback);

	//Wait to open the socket
	std::cout << "Press ENTER to get LOGIN profile" << std::endl;	
	std::cin.get(enterKey);

	//Call AUTHENTICATE method
	oBalancer->getUser(loginID_, getLoginCallback);

	//Wait to open the socket
	std::cout << "Press ENTER to notify for list users" << std::endl;
	std::cin.get(enterKey);

	//Call NOTIFY_LIST_USERS method
	std::vector<std::string> arrUserID;
	arrUserID.push_back(loginID_);
	arrUserID.push_back(memberID_);
	int code = 1;
	std::string reason = "Notify for " + loginID_ + "," + memberID_;
	bool ignoreAlertMeFlg = false;
	oBalancer->notifyListUser(arrUserID, code, reason, ignoreAlertMeFlg, notifyUsersCallback);

	//Wait to close the socket	
	std::cout << "Press ENTER to release and close SOCKET" << std::endl;
	std::cin.get(enterKey);

	//Destroy instance
	oBalancer->release();

	return 0;
}

