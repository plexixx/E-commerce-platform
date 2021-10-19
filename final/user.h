#ifndef USER_H
#define USER_H

#include<iostream>
#include<string>
#include<map>
#include<list>
#include<vector>

#include"item.h"

using namespace std;

class User
{
public:
	bool ifValidUserPass(const string& name, const string& password);
	bool setPass(const string& newpw); 
	bool addBalance(double b);
	bool subBalance(double b);
	
	string getPassword() { return password; }
	string getName() { return name; }
	double getBalance() { return balance; }

	virtual int getUserType() = 0;

	User() = default;
	User(const string& nm, const string& pw, double bal) :
		name(nm), password(pw), balance(bal) {};
	~User() = default;

protected:
	string name;
	string password;
	double balance = 0; // 余额
};

class Trader : public User
{
private:
	int type = TRADER; // User's type，Trader = 2，User = 1
	 
public:
	virtual int getUserType() { return type; }

	Trader()=default;
	Trader(const string& nm, const string& pw, double bal) :
		User(nm,pw,bal) {};
	~Trader()=default;
};

class Customer :public User
{
private:
	int type = CUSTOMER;

public:
	map<string, unsigned int> cart;  // < 商品名, 数量 >
	map<string, unsigned int> order;

	virtual int getUserType() { return type; }

	bool addToCart(const string& name, unsigned int num);
	bool removeFromCart(const string& name);
	bool removeFromCart(const string& name, unsigned int num);
	bool setItemNum(const string& name, unsigned int num);
	bool generateOrder(const string& name, unsigned int stock);
	

	Customer()=default;
	Customer(const string& nm, const string& pw, double bal) :
		User(nm, pw, bal) {};
	~Customer()=default;
};

#endif