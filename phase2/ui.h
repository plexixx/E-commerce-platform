#ifndef UI_H
#define UI_H

#include<iostream>
#include"global.h"
#include"system.h"

class userInterface
{
private:
	unsigned int userType = 0;
	bool ifLogin = false;
	string username_t;
	ESystem& sys = ESystem::getInstance();

	bool login();
	bool setPass();
	bool recharge();
	bool registerUser();
	bool addItem();
	bool editItemInfo();
	bool delUser();
	bool delItem();
	
	void displayInfo() const;
	void sortItemList() const;
	void viewItemList() const;
	void viewCart() const;
	void viewOrder() const;
	void viewDetail(const string& name) const;

	void crInterface();
	void trInterface();

public:
	userInterface();
	~userInterface() = default;
	void show();
};


#endif
