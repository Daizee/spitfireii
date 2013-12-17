//
// server.hpp
// Project Spitfire
//
// Copyright (c) 2013 Daizee (rensiadz at gmail dot com)
//
// This file is part of Spitfire.
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef SPITFIRE_SERVER_HPP
#define SPITFIRE_SERVER_HPP

//#include <iostream>
//#include <list>
//#include <string>
//#include <cctype>

//#include "sfsql.h"
#include "structs.h"
#include "funcs.h"
#include "connection.h"
#include <set>

#include <lua/lua.hpp>
#include <lua/lauxlib.h>


#include <boost/thread.hpp>
#include <asio.hpp>
// #include <iostream>
// #include <list>
// #include <string>
// #include <cctype>
#include <boost/noncopyable.hpp>

class Map;
class Client;
class City;
class Alliance;
class AllianceCore;
class sockets;


#include "Poco/Data/Common.h"
//#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/MySQL/Connector.h"
#include "Poco/Data/MySQL/MySQLException.h"
#include "Poco/Data/SessionPool.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/Exception.h"

#include "Poco/Logger.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/Message.h"

using Poco::Logger;
using Poco::PatternFormatter;
using Poco::FormattingChannel;
using Poco::ConsoleChannel;
using Poco::FileChannel;
using Poco::Message;
using Poco::Data::RecordSet;

using namespace std;


using namespace Poco::Data;


/// The top-level class of the Spitfire server.
class Server
{
public:
	Server();
	~Server();


	/// Run the server's io_service loop.
	void run();

	/// Stop the server.
	void stop();

	/// Start the first asynchronous operation for the connection.
	void start(connection_ptr c);

	/// Stop the specified connection.
	void stop(connection_ptr c);

	/// Stop all connections.
	void stop_all();

	/// Handle completion of an asynchronous accept operation.
	void handle_accept(const asio::error_code& e);

	/// Handle a request to stop the server.
	void handle_stop();

	/// The io_service used to perform asynchronous operations.
	asio::io_service io_service_;

	/// The signal_set is used to register for process termination notifications.
	asio::signal_set signals_;

	/// Acceptor used to listen for incoming connections.
	asio::ip::tcp::acceptor acceptor_;

	/// The connection list which owns all live connections.
	std::set<connection_ptr> connections_;

	/// The next connection to be accepted.
	connection_ptr new_connection_;

	/// The handler for all incoming requests.
	request_handler request_handler_;


	// MySQL
	string sqlhost, sqluser, sqlpass, bindaddress, bindport;
	string dbmaintable;
	string dbservertable;

	uint32_t serverstatus;

	// Map size -- typically 500x500 or 800x800
	uint16_t mapsize;

	// Console and File log formatting
	FormattingChannel* pFCConsole;
	FormattingChannel* pFCFile;

	// Console and File log handles
	Logger * consoleLogger;
	Logger * fileLogger;

	// MySQL connection pools
	SessionPool * accountpool;
	SessionPool * serverpool;


	// Lua Handle
	lua_State *L;

	// Initialize Server
	bool Init();

	// Max players allowed connected
	uint32_t maxplayers;

	// Current players connected
	uint32_t currentplayersonline;

	// Whether the timer thread is running or not
	bool TimerThreadRunning;

	void TimerThread();
	void SaveData();

	string servername;
	Map * map;
	bool ConnectSQL();
	bool InitSockets();
	Client * NewClient();
	Client * GetClient(int accountid);
	Client * GetClientByParent(int accountid);
	Client * GetClientByName(string name);
	Client * GetClientByCastle(int32_t castleid);
	int32_t  GetClientIndex(int32_t accountid);
	void CloseClient(int id);
	void CloseClient(Client * client, int typecode = 1, string message = "Connection Closed");

	City * AddPlayerCity(Client * client, int tileid, uint32_t castleid);
	City * AddNpcCity(int tileid);
	void MassMessage(string str, bool nosender = false, bool tv = false, bool all = false);
	void SendMessage(Client * client, string str, bool nosender = false, bool tv = false, bool all = false);
	void Shutdown();

	list<Client*> players;

	bool savethreadrunning;
	sockets * skts;

	void SendObject(connection * s, amf3object & object)
	{
		if (s == 0)
			return;
		SendObject(*s, object);
	}
	void SendObject(connection & s, amf3object & object)
	{
		try
		{
			if (serverstatus == 0)
				return;
			char buffer[15000];
			int length = 0;
			amf3writer * writer;

			writer = new amf3writer(buffer+4);

			writer->Write(object);

			(*(int*)buffer) = length = writer->position;
			ByteSwap(*(int*)buffer);

			s.write(buffer, length+4);
			delete writer;
		}
		catch (std::exception& e)
		{
			std::cerr << "exception: " << __FILE__ << " @ " << __LINE__ << "\n";
			std::cerr << e.what() << "\n";
		}
	}

	// List of active cities on the server (NPC and Player)
	vector<City*> m_city;

	// Alliance Controller
	AllianceCore * m_alliances;

	// Parse chat for commands
	bool ParseChat(Client * client, char * str);

	// Get Alliance based relation between two clients
	int16_t GetRelation(int32_t client1, int32_t client2);

	uint64_t ltime;

	uint64_t armycounter;
	string m_itemxml;
	stItemConfig m_items[DEF_MAXITEMS];
	int m_itemcount;
	stRarityGamble m_gambleitems;



	list<stMarketEntry> m_marketbuy;
	list<stMarketEntry> m_marketsell;

	static bool comparebuy(stMarketEntry first, stMarketEntry second);
	static bool comparesell(stMarketEntry first, stMarketEntry second);
	void AddMarketEntry(stMarketEntry me, int8_t type);

#define DEF_TIMEDARMY 1
#define DEF_TIMEDBUILDING 2
#define DEF_TIMEDRESEARCH 3

	stItemConfig * GetItem(string name);

	double GetPrestigeOfAction(int8_t action, int8_t id, int8_t level, int8_t thlevel);


	void MassDisconnect();
	void AddTimedEvent(stTimedEvent & te);


	stBuildingConfig m_buildingconfig[35][10];
	stBuildingConfig m_researchconfig[25][10];
	stBuildingConfig m_troopconfig[20];

	list<stTimedEvent> armylist;
	list<stTimedEvent> buildinglist;
	list<stTimedEvent> researchlist;

	queue<stPacketOut> m_packetout;

	int64_t m_heroid;
	int64_t m_cityid;
	int32_t m_allianceid;

	vector<int32_t> m_deletedhero;
	vector<int32_t> m_deletedcity;

	int RandomStat();


	Hero * CreateRandomHero(int innlevel);



	static bool compareprestige(stClientRank first, stClientRank second);
	static bool comparehonor(stClientRank first, stClientRank second);
	static bool comparetitle(stClientRank first, stClientRank second);
	static bool comparepop(stClientRank first, stClientRank second);
	static bool comparecities(stClientRank first, stClientRank second);

	void SortPlayers();


	list<stClientRank> m_prestigerank;
	list<stClientRank> m_honorrank;
	list<stClientRank> m_titlerank;
	list<stClientRank> m_populationrank;
	list<stClientRank> m_citiesrank;



	list<stHeroRank> m_herorankstratagem;
	list<stHeroRank> m_herorankpower;
	list<stHeroRank> m_herorankmanagement;
	list<stHeroRank> m_herorankgrade;

	void SortHeroes();
	static bool comparestratagem(stHeroRank first, stHeroRank second);
	static bool comparepower(stHeroRank first, stHeroRank second);
	static bool comparemanagement(stHeroRank first, stHeroRank second);
	static bool comparegrade(stHeroRank first, stHeroRank second);


	list<stCastleRank> m_castleranklevel;
	list<stCastleRank> m_castlerankpopulation;

	void SortCastles();
	static bool comparepopulation(stCastleRank first, stCastleRank second);
	static bool comparelevel(stCastleRank first, stCastleRank second);


	list<stSearchClientRank> m_searchclientranklist;
	list<stSearchHeroRank> m_searchheroranklist;
	list<stSearchCastleRank> m_searchcastleranklist;
	list<stSearchAllianceRank> m_searchallianceranklist;

	void * DoRankSearch(string key, int8_t type, void * subtype, int16_t page, int16_t pagesize);
	void CheckRankSearchTimeouts(uint64_t time);


	// Construct an error message to send to the client
	amf3object CreateError(string cmd, int32_t id, string message)
	{
		amf3object obj;
		obj["cmd"] = cmd;
		obj["data"] = amf3object();
		amf3object & data = obj["data"];
		data["errorMsg"] = message;
		data["packageId"] = 0.0f;
		data["ok"] = id;
		return obj;
	}
	// Construct an error message to send to the client
	// tested and functioning 
	// gserver->SendObject(&c, *gserver->CreateError2("error.error", -1, "message").get());
	shared_ptr<amf3object> CreateError2(string cmd, int32_t id, string message)
	{
		shared_ptr<amf3object> obj(new amf3object);
		obj->operator[]("cmd") = cmd;
		obj->operator[]("data") = amf3object();
		amf3object & data = obj->operator[]("data");
		data["errorMsg"] = message;
		data["packageId"] = 0.0f;
		data["ok"] = id;
		return obj;
	}
};

#endif // SPITFIRE_SERVER_HPP
