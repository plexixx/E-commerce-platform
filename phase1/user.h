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
	using string = Shared::User;
	bool ifValidUserPass(const string& name, const string& password);
	bool setPass(const string& newpw); 
	bool addBalance(double b);
	
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
	double balance = 0; // ”‡∂Ó
};

class Trader : public User
{
private:
	int type = TRADER; // User's type£¨Trader = 2£¨User = 1
	 
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

	virtual int getUserType() { return type; }

	Customer()=default;
	Customer(const string& nm, const string& pw, double bal) :
		User(nm, pw, bal) {};
	~Customer()=default;
};

#endif