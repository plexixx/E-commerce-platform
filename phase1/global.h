#ifndef GLOBAL_H
#define GLOBAL_H

#include <memory>
#include <stdexcept>
#include <iostream>
#include <string>

class User;
class Trader;
class Customer;
class Item;
class Book;
class Cloth;
class Food;

typedef std::shared_ptr<User> User_ptr;
typedef std::shared_ptr<Trader> Trader_ptr;
typedef std::shared_ptr<Customer> Customer_ptr;
typedef std::shared_ptr<Item> Item_ptr;
typedef std::shared_ptr<Book> Book_ptr;
typedef std::shared_ptr<Cloth> Cloth_ptr;
typedef std::shared_ptr<Food> Food_ptr;


constexpr unsigned short int LOGIN = 1;
constexpr unsigned short int REG = 2;
constexpr unsigned short int USER_INFO = 3;
constexpr unsigned short int RESET_PW = 4;
constexpr unsigned short int RECHARGE = 5;
constexpr unsigned short int ADD_ITEM = 6;
constexpr unsigned short int EDIT_ITEM_INFO = 7;
constexpr unsigned short int DEL_ITEM = 8;
constexpr unsigned short int LIST_ALL_ITEM = 9;
constexpr unsigned short int SORT_ITEM = 10;
constexpr unsigned short int LOGOUT = 11;
constexpr unsigned short int DEACTIVE = 12;
constexpr unsigned short int EXIT = 14;
constexpr unsigned short int CART = 6;
constexpr unsigned short int ORDER_LIST = 7;

constexpr unsigned short int SET_DES = 1;
constexpr unsigned short int SET_STOCK = 2;
constexpr unsigned short int SET_PRICE = 3;
constexpr unsigned short int SET_DIS = 4;
constexpr unsigned short int SET_TDIS = 5;


constexpr int CUSTOMER = 1;
constexpr int TRADER = 2;
constexpr int BOOK = 1;
constexpr int CLOTH = 2;
constexpr int FOOD = 3;

constexpr unsigned short int MAX_STOCK = 9999;
constexpr unsigned short int MIN_STOCK = 0;

bool processError(std::invalid_argument);

namespace Shared
{
	typedef std::string User;
	typedef std::string Item;
}


#endif // !GLOBAL_H

