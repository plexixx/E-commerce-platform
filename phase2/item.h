#ifndef ITEM_H
#define ITEM_H

#include<iostream>
#include<iomanip>
#include<string>

#include"global.h"

using namespace std;

class Item
{
public:
	bool setItemName(const string& name);
	bool setDescript(const string& des);
	bool setDiscount(const double& dis);
	bool setStock(const unsigned int& stk);
	bool setOwner(const string& username);
	bool setOrigin(double ori);

	string getItemName() const { return name; }
	string getDescript() const { return descript; }
	string getOwner() const { return owner; }
	double getOrigin() const { return origin; }
	double getDiscount() const { return discount; }
	unsigned int getStock() const { return stock; }

	void displayItemInfo();

	virtual double getPrice() = 0;
	virtual int getItemType() = 0;

	Item() = default;
	Item(const string& nm, const string& un, const string& des, double ori, unsigned int stk, double dis) :
		name(nm), owner(un), descript(des), origin(ori), stock(stk), discount(dis) {};
	~Item() = default;

protected:
	string name;
	string descript;
	string owner;
	double origin=0;
	double discount=1.0;
	unsigned int stock=9999;
	int type = 0; // Book -- 1, Cloth -- 2, Food -- 3
};

class Book :public Item
{	
public:
	static double dis; // 单品类全部商品打折
	virtual int getItemType() { return BOOK; }
	virtual double getPrice() { return dis * origin * discount; }

	static bool setDiscount(double d);

	Book() = default;
	Book(const string& nm, const string& un, const string& des, double ori, unsigned int stk, double dis) :
		Item(nm, un, des, ori, stk, dis) {};
	~Book() = default;
};

class Cloth :public Item
{
public:
	static double dis;
	virtual int getItemType() { return CLOTH; }
	virtual double getPrice() { return dis * origin * discount; }

	static bool setDiscount(double d);

	Cloth() = default;
	Cloth(const string& nm, const string& un, const string& des, double ori, unsigned int stk, double dis) :
		Item(nm, un, des, ori, stk, dis) {};
	~Cloth() = default;
};

class Food :public Item
{
public:
	static double dis;
	virtual int getItemType() { return FOOD; }
	virtual double getPrice() { return dis * origin * discount; }

	static bool setDiscount(double d);

	Food()=default;
	Food(const string& nm, const string& un, const string& des, double ori, unsigned int stk, double dis) :
		Item(nm, un, des, ori, stk, dis) {};
	~Food()=default;
};

#endif