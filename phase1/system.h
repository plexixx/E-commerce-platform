#ifndef SYSTEM_H
#define SYSTEM_H

#include<iostream>
#include<map>
#include<memory>
#include <string>
#include<fstream>

#include"global.h"
#include"item.h"
#include"user.h"

using namespace std;

class ESystem
{
private:
	ESystem();
	static unique_ptr<ESystem> instance;

	map<string, User_ptr>nameToUser;
	map<string, Item_ptr>nameToItem;

public:
	static ESystem& getInstance() // 确保全局只有唯一实例
	{
		if (instance == nullptr)
			instance.reset(new ESystem);
		return *instance;
	}
	
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
	bool checkPass(const string& username, const string& password) const;
	bool setPass(const string& username, const string& password);
	unsigned int getUserType(const string& username) const;		

	// 商品管理
	
	bool addItem(const string& name, const string& username, const string& des, double origin, unsigned int stock, int type);
	bool delItem(const string& username, const string& name);
	bool delItem(const string& username);
	bool ifHaveItem(const string& name, const string& username) const;
	void listItem() const;
	void listItem(int type) const;
	bool displayItemInfo(const string& name) const;
	bool setOrigin(const string& username, const string& name, double p);
	bool setDiscount(const string& username, const string& name, double dis);
	bool setDiscount(const string& username, int type, double dis);
	bool setStock(const string& username, const string& name, unsigned int stk);
	bool setDescript(const string& username, const string& name, const string& des);
	int getItemType(const string& name, const string& username) const;
	

	~ESystem();


};


#endif