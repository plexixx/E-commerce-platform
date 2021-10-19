#ifndef SYSTEM_H
#define SYSTEM_H

#include<iostream>
#include<fstream>
#include<sstream>
#include<map>
#include<memory>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>  

#include"global.h"
#include"item.h"
#include"user.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

class ESystem
{
private:
	ESystem();
	static unique_ptr<ESystem> instance;

	map<string, User_ptr>nameToUser;
	map<string, Item_ptr>nameToItem;
	map<int, string>tokenToUser;

public:
	static ESystem& getInstance() // 确保全局只有唯一实例
	{
		if (instance == nullptr)
			instance.reset(new ESystem);
		return *instance;
	}

	int getValue(const string& username);

	// 数据存取

	bool readUserFile(const string& fp = "user.txt");
	bool saveUserFile(const string& fp = "user.txt") const;
	bool readItemFile(const string& fp = "item.txt");
	bool saveItemFile(const string& fp = "item.txt") const;

	// 用户管理

	bool addUser(const string& username, const string& password, unsigned int userType);
	bool delUser(const string& username);
	bool ifHaveUser(const string& username) const;
	bool displayUserInfo(const string& username) const;
	bool addBalance(const string& username, double bal);
	bool subBalance(const string& username, double bal);
	bool checkPass(const string& username, const string& password) const;
	bool setPass(const string& username, const string& password);
	unsigned int getUserType(const string& username) const;
	double getBalance(const string& username) const;
	

	// 商品管理

	bool addItem(const string& name, const string& username, const string& des, double origin, unsigned int stock, int type);
	bool delItem(const string& username, const string& name);
	bool delItem(const string& username);
	bool ifHaveItem(const string& name) const;
	void listItem() const;
	void listItem(int type) const;
	bool displayItemInfo(const string& name) const;
	bool setOrigin(const string& username, const string& name, double p);
	bool setDiscount(const string& username, const string& name, double dis);
	bool setDiscount(const string& username, int type, double dis);
	bool setStock(const string& username, const string& name, unsigned int stk);
	bool setDescript(const string& username, const string& name, const string& des);
	int getItemType(const string& name) const;
	unsigned int getStock(const string& name) const;
	double getOrigin(const string& name) const;
	double getPrice(const string& name) const;
	string getOwner(const string& name) const;
	string getDescript(const string& name) const;
	
	// 购物
	
	bool pay(const string& username);
	bool displayCart(const string& username) const;
	double displayOrder(const string& username) const;
	bool addToCart(const string& name, const string& username, unsigned int num);
	bool removeFromCart(const string& name, const string& username);
	bool removeFromCart(const string& name, const string& username, unsigned int num);
	bool clearCart(const string& username);
	bool generateOrder(const string& name, const string& username);
	bool delOrder(const string& username);
	bool setItemQuantity(const string& name, const string& username, unsigned int num);
		
	void run(const string& port);

	~ESystem();


};


#endif