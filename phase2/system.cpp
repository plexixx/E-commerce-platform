#include"system.h"

unique_ptr<ESystem> ESystem::instance = nullptr;

using namespace std;

ESystem::ESystem()
{
	Book::setDiscount(1.0);
	Cloth::setDiscount(1.0);
	Food::setDiscount(1.0);
}

ESystem::~ESystem()
{

}

// ------------- 数据存取 ------------- 

// 读取用户信息
bool ESystem::readUserFile(const string& fp)
{
	ifstream ifs(fp, ios::in);
	if (!ifs.good())
	{
		cout << "数据读取失败!\n";
		system("pause");
		return false;
	}
	string tpname, tppw;
	int tptype;
	double tpbal;
	ifs >> tpname;
	while (!ifs.eof())
	{
		ifs >> tppw >> tptype >> tpbal;
		User_ptr u = nullptr;
		switch (tptype)
		{
		case CUSTOMER:
			u = Customer_ptr(new Customer(tpname, tppw, tpbal));
			break;
		case TRADER:
			u = Trader_ptr(new Trader(tpname, tppw, tpbal));
			break;
		}
		if (u != nullptr)nameToUser.insert(pair<string, User_ptr>(tpname, u));
		ifs >> tpname;
	}
	ifs.close();
	cout << "用户数据读取成功.\n";
	system("pause");
	return true;
}

// 存储用户信息
bool ESystem::saveUserFile(const string& fp) const
{
	ofstream ofs(fp, ios::out);
	for (auto it = nameToUser.begin(); it != nameToUser.end(); it++)
	{
		int type = it->second->getUserType();
		ofs << it->second->getName() << " " 
			<<it->second->getPassword()<< " " 
			<< type << " " 
			<< it->second->getBalance() << endl;
	}
	ofs.close();
	cout << "用户数据存储成功.\n";
	system("pause");
	return true;
}

// 读取商品信息
bool ESystem::readItemFile(const string& fp)
{
	ifstream ifs(fp, ios::in);
	if (!ifs.good())
	{
		cout << "数据读取失败!\n";
		system("pause");
		return false;
	}
	string tpname, tpowner, tpdes;
	unsigned int tpstk;
	int tptype;
	double tporigin,tpdis;
	ifs >> tpname;
	while (!ifs.eof())
	{
		ifs >> tptype >> tpowner >> tporigin >> tpdis >> tpstk >> tpdes;
		Item_ptr u = nullptr;
		switch (tptype)
		{
		case BOOK:
			u = Book_ptr(new Book(tpname, tpowner, tpdes, tporigin, tpstk, tpdis));
			break;
		case CLOTH:
			u = Cloth_ptr(new Cloth(tpname, tpowner, tpdes, tporigin, tpstk, tpdis));
			break;
		case FOOD:
			u = Food_ptr(new Food(tpname, tpowner, tpdes, tporigin, tpstk, tpdis));
			break;
		}
		if (u != nullptr)nameToItem.insert(pair<string, Item_ptr>(tpname, u));
		ifs >> tpname;
	}
	ifs.close();
	cout << "商品数据读取成功.\n";
	system("pause");
	return true;
}

// 存储商品信息
bool ESystem::saveItemFile(const string& fp) const
{
	ofstream ofs(fp, ios::out);
	for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
	{
		int type = it->second->getItemType();
		ofs << it->second->getItemName() << " "
			<< type << " "
			<< it->second->getOwner() << " "
			<< it->second->getOrigin() << " "
			<< it->second->getDiscount() << " "
			<< it->second->getStock() << " "
			<< it->second->getDescript() << endl;
	}
	ofs.close();
	cout << "商品数据存储成功.\n";
	system("pause");
	return true;
}


// ------------- 用户管理 ------------- 

// 添加用户
bool ESystem::addUser(const string& username, const string& password, unsigned int userType)
{
	if (ifHaveUser(username)) 
	{
		cout << "用户名已注册!\n";
		return false;
	}

	// 检查输入合法性
	if (username.length() < 5 || username.length() > 10)
	{
		cout << "Invalid name!\nPlease enter a new name.(5-10)\n\n";
		return false;
	}
	if (password.length() < 6 || password.length() > 15)
	{
		cout << "Invalid Password!\nPlease reenter the security password.(6-15)\n\n";
		return false;
	}
	if (password.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos
		|| username.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		std::cerr << "Error!\nname/Password can only contain a-z, A-Z, 0-9 and _\n\n";
		return false;
	}

	User_ptr u = nullptr;
	switch (userType)
	{
	case CUSTOMER:
		u = Customer_ptr(new Customer(username, password, 0));
		break;
	case TRADER:
		u = Trader_ptr(new Trader(username, password, 0));
		break;
	}
	if (u != nullptr)
	{
		nameToUser.insert(pair<string, User_ptr>(username, u));
		return true;
	}
	return false;
		
}

// 删除用户
bool ESystem::delUser(const string& username)
{
	for (auto it = nameToUser.begin(); it != nameToUser.end(); it++)
	{
		if (username.compare("admin") == 0)
		{
			cout << "管理员账号不可删除!\n";
			return false;
		}
		if (username.compare(it->first) == 0)
		{
			if (it->second->getUserType() == TRADER)
				delItem(username);
			
			nameToUser.erase(username);
			return true;
		}
	}
	return false;
}

// 判断用户是否存在
bool ESystem::ifHaveUser(const string& username) const
{
	for (auto it = nameToUser.begin(); it != nameToUser.end(); it++)
	{
		if (username.compare(it->first) == 0)
			return true;
	}
	return false;
}

// 展示用户个人信息
bool ESystem::displayUserInfo(const string& username) const
{
	if (ifHaveUser(username))
	{
		for (auto it = nameToUser.begin(); it != nameToUser.end(); it++)
		{
			if (username.compare(it->first) == 0)
			{
				cout << "用户信息:\n用户名:" << it->first
					<< "\n用户类型:" << it->second->getUserType()
					<< "\t( 1 -- 普通用户, 2 -- 商家 )"
					<< "\n账户余额:" << it->second->getBalance() << endl;
				return true;
			}
		}
	}
	return false;
}

// 余额增加
bool ESystem::addBalance(const string& username, double bal)
{
	for (auto it = nameToUser.begin(); it != nameToUser.end(); it++)
	{
		if (username.compare(it->first) == 0)
		{
			if (it->second->addBalance(bal))
				return true;
		}
	}
	return false;
}

// 余额减少
bool ESystem::subBalance(const string& username, double bal)
{
	for (auto it = nameToUser.begin(); it != nameToUser.end(); it++)
	{
		if (username.compare(it->first) == 0)
		{
			if (it->second->subBalance(bal))
				return true;
		}
	}
	return false;
}

// 登录检查
bool ESystem::checkPass(const string& username, const string& password) const
{
	if (username.compare("admin") == 0)
		if (password.compare("admin") == 0)
			return true;
	for (auto it = nameToUser.begin(); it != nameToUser.end(); it++)
	{
		if (username.compare(it->first) == 0)
		{
			if (password.compare(it->second->getPassword()) == 0)
				return true;
			return false;
		}
	}
	return false;
}

// 设置密码
bool ESystem::setPass(const string& username, const string& password)
{
	for (auto it = nameToUser.begin(); it != nameToUser.end(); it++)
	{
		if (username.compare(it->first) == 0)
		{
			if (it->second->setPass(password))
				return true;
		}	
	}
	return false;
}

// 获取用户类型
unsigned int ESystem::getUserType(const string& username) const
{
	unsigned int tp = 0;
	for (auto it = nameToUser.begin(); it != nameToUser.end(); it++)
	{
		if (username.compare(it->first) == 0)
		{
			tp = it->second->getUserType();
		}
	}
	return tp;
}


// ------------- 商品管理 ------------- 

// 添加商品
bool ESystem::addItem(const string& name, const string& username, const string& des, double origin, unsigned int stock, int type)
{
	for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
	{
		if (name.compare(it->first) == 0 && username.compare(it->second->getOwner()))
		{
			cout << "商品已存在!\n";
			return false;
		}
	}
	Item_ptr i = nullptr;
	switch (type)
	{
	case BOOK:
		i = Book_ptr(new Book(name, username, des, origin, stock, 1.0));
		break;
	case CLOTH:
		i = Cloth_ptr(new Cloth(name, username, des, origin, stock, 1.0));
		break;
	case FOOD:
		i = Food_ptr(new Food(name, username, des, origin, stock, 1.0));
		break;
	}
	if (i != nullptr)
	{
		nameToItem.insert(pair<string, Item_ptr>(name, i));
		return true;
	}
	return false;
}

// 删除商品
bool ESystem::delItem(const string& username, const string& name)
{
	for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
	{
		if (name.compare(it->first) == 0)
		{
			if (username.compare(it->second->getOwner()) == 0 || username.compare("admin") == 0)
			{
				nameToItem.erase(name);
				return true;
			}
		}
	}
	return false;
}

// 删除商家所有商品
bool ESystem::delItem(const string& username)
{
	string name;
	for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
	{
		if (username.compare(it->second->getOwner()) == 0 || username.compare("admin") == 0)
		{
			name = it->first;
			nameToItem.erase(name);
			return true;
		}
	}
	return false;
}

// 判断商品是否存在
bool ESystem::ifHaveItem(const string& name) const
{
	for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
	{
		if (name.compare(it->first) == 0)
			return true;
	}
	return false;
}

// 展示所有商品
void ESystem::listItem() const
{
	cout << setw(20) << "商品名称"
		<< setw(20) << "价格"
		<< setw(20) << "类型"
		<< setw(20) << "库存" 
		<< setw(20) << "商家"
		<< endl;
	for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
	{
		int type = it->second->getItemType();
		cout << setw(20) << it->first
			<< setw(20) << it->second->getPrice()
			<< setw(20) << type
			<< setw(20) << it->second->getStock() 
			<< setw(20) << it->second->getOwner()
			<< endl;
	}
}

// 展示指定品类商品
void ESystem::listItem(int type) const
{
	cout << setw(25) << "商品名称"
		<< setw(25) << "价格"
		<< setw(25) << "类型"
		<< setw(25) << "库存" << endl;
	for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
	{
		if (type == it->second->getItemType())
		{
			cout << setw(25) << it->first
				<< setw(25) << it->second->getPrice()
				<< setw(25) << type
				<< setw(25) << it->second->getStock() << endl;
		}
	}
}

// 展示商品详细信息
bool ESystem::displayItemInfo(const string& name) const
{
	for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
	{
		if (name.compare(it->first) == 0)
		{
			int type = it->second->getItemType();
			double price = it->second->getPrice();
			cout << "商品信息:\n商品名称:" << it->first
				<< "\n商家:" << it->second->getOwner()
				<< "\n原价:" << it->second->getOrigin()
				<< "\n现价:" << price
				<< "\n库存:" << it->second->getStock()
				<< "\n商品描述:" << it->second->getDescript()
				<< endl;
			return true;
		}
	}
	return false;
}

// 设置商品原价
bool ESystem::setOrigin(const string& username, const string& name, double p)
{
	for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
	{
		if (name.compare(it->first) == 0)
		{
			if (username.compare(it->second->getOwner()) == 0 || username.compare("admin") == 0)
			{
				if (it->second->setOrigin(p))
					return true;
			}
		}
	}
	return false;
}

// 设置商品折扣
bool ESystem::setDiscount(const string& username, const string& name, double dis)
{
	for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
	{
		if (name.compare(it->first) == 0)
		{
			if (username.compare(it->second->getOwner()) == 0 || username.compare("admin"))
			{
				if (it->second->setDiscount(dis))
					return true;
			}
		}
	}
	return false;
}

// 设置品类折扣
bool ESystem::setDiscount(const string& username, int type, double dis)
{
	if (username.compare("admin") == 0)
	{
		switch (type)
		{
		case BOOK:
			Book::setDiscount(dis);
			return true;
			break;
		case CLOTH:
			Cloth::setDiscount(dis);
			return true;
			break;
		case FOOD:
			Food::setDiscount(dis);
			return true;
			break;
		}
	}
	return false;
}

// 设置商品库存
bool ESystem::setStock(const string& username, const string& name, unsigned int stk)
{
	for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
	{
		if (name.compare(it->first) == 0)
		{
			if (username.compare(it->second->getOwner()) == 0 || username.compare("admin") == 0)
			{
				if (it->second->setStock(stk))
					return true;
			}
		}
	}
	return false;
}

// 设置商品描述
bool ESystem::setDescript(const string& username, const string& name, const string& des)
{
	for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
	{
		if (name.compare(it->first) == 0)
		{
			if (username.compare(it->second->getOwner()) == 0 || username.compare("admin") == 0)
			{
				if (it->second->setDescript(des))
					return true;
			}
		}
	}
	return false;
}

// 获取商品类型
int ESystem::getItemType(const string& name) const
{
	for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
	{
		if (name.compare(it->first) == 0)
			return it->second->getItemType();
	}
	return 0;
}

// 获取商品价格
double ESystem::getPrice(const string& name) const
{
	for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
	{
		if (name.compare(it->first) == 0)
			return it->second->getPrice();
	}
	return 0;
}

// 获取商品商家
string ESystem::getOwner(const string& name) const
{
	for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
	{
		if (name.compare(it->first) == 0)
			return it->second->getOwner();
	}
	return 0;
}

// 获取商品库存
unsigned int ESystem::getStock(const string& name) const
{
	for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
	{
		if (name.compare(it->first) == 0)
			return it->second->getStock();
	}
	return 0;
}



// ------------- 购物 ------------- 

// 展示购物车
bool ESystem::displayCart(const string& username) const
{
	if (ifHaveUser(username))
	{
		for (auto it = nameToUser.begin(); it != nameToUser.end(); it++)
		{
			if (username.compare(it->first) == 0 && it->second->getUserType() == CUSTOMER)
			{
				// 购物车为空
				if ((dynamic_pointer_cast<Customer>(it->second))->cart.size() == 0)
					return false;
				for (auto is = (dynamic_pointer_cast<Customer>(it->second))->cart.begin(); is != (dynamic_pointer_cast<Customer>(it->second))->cart.end(); is++)
				{
					cout << setw(15) << is-> first
						<< setw(15) << is->second << endl;
				}
				return true;
			}
		}
	}
	return false;
}

// 展示订单
double ESystem::displayOrder(const string& username) const
{
	double sum = 0;
	if (ifHaveUser(username))
	{
		for (auto it = nameToUser.begin(); it != nameToUser.end(); it++)
		{
			if (username.compare(it->first) == 0 && it->second->getUserType() == CUSTOMER)
			{
				for (auto is = (dynamic_pointer_cast<Customer>(it->second))->order.begin(); is != (dynamic_pointer_cast<Customer>(it->second))->order.end(); is++)
				{
					cout << setw(15) << is->first
						<< setw(15) << is->second << endl;
					sum += getPrice(is->first) * is->second;
				}
			}
		}
	}
	return sum;
}

// 添加商品至购物车
bool ESystem::addToCart(const string& name, const string& username, unsigned int num)
{
	if (num < 0)
		return false;
	if (ifHaveItem(name) && ifHaveUser(username))
	{
		for (auto it = nameToUser.begin(); it != nameToUser.end(); it++)
		{
			if (username.compare(it->first) == 0 && it->second->getUserType() == CUSTOMER)
			{
				Customer_ptr c = dynamic_pointer_cast<Customer> (it->second);
				if (c->addToCart(name, num))
					return true;
			}
		}
	}
	return false;
}

// 移除购物车内商品
bool ESystem::removeFromCart(const string& name, const string& username)
{
	if (ifHaveItem(name) && ifHaveUser(username))
	{
		for (auto it = nameToUser.begin(); it != nameToUser.end(); it++)
		{
			if (username.compare(it->first) == 0 && it->second->getUserType() == CUSTOMER)
			{
				Customer_ptr c = dynamic_pointer_cast<Customer> (it->second);
				if (c->removeFromCart(name))
					return true;
			}
		}
	}
	return false;
}

// 移除购物车内一定数目的商品
bool ESystem::removeFromCart(const string& name, const string& username, unsigned int num)
{
	if (num < 0)
		return false;
	if (ifHaveItem(name) && ifHaveUser(username))
	{
		for (auto it = nameToUser.begin(); it != nameToUser.end(); it++)
		{
			if (username.compare(it->first) == 0 && it->second->getUserType() == CUSTOMER)
			{
				Customer_ptr c = dynamic_pointer_cast<Customer> (it->second);
				if (c->removeFromCart(name, num))
					return true;
			}
		}
	}
	return false;
}

// 清空购物车
bool ESystem::clearCart(const string& username)
{
	if (ifHaveUser(username))
	{
		for (auto it = nameToUser.begin(); it != nameToUser.end(); it++)
		{
			if (username.compare(it->first) == 0 && it->second->getUserType() == CUSTOMER)
			{
				Customer_ptr c = dynamic_pointer_cast<Customer> (it->second);
				c->cart.clear();
				return true;
			}
		}
	}
	return false;
}

// 生成订单
bool ESystem::generateOrder(const string& name, const string& username)
{
	if (ifHaveUser(username))
	{
		for (auto it = nameToUser.begin(); it != nameToUser.end(); it++)
		{
			if (username.compare(it->first) == 0 && it->second->getUserType() == CUSTOMER)
			{
				Customer_ptr c = dynamic_pointer_cast<Customer> (it->second);
				if (c->generateOrder(name,getStock(name)))
				{
					// 冻结商品对应库存
					for (auto is = c->order.begin(); is != c->order.end(); is++)
						for (auto item = nameToItem.begin(); item != nameToItem.end(); item++)
							if (is->first.compare(item->first) == 0)
								item->second->setStock(item->second->getStock() - is->second);
					return true;
				}
			}
		}
	}
	return false;
}

// 删除订单
bool ESystem::delOrder(const string& username)
{
	if (ifHaveUser(username))
	{
		for (auto it = nameToUser.begin(); it != nameToUser.end(); it++)
		{
			if (username.compare(it->first) == 0 && it->second->getUserType() == CUSTOMER)
			{
				Customer_ptr c = dynamic_pointer_cast<Customer> (it->second);
				for (auto is = c->order.begin(); is != c->order.end(); is++)
					for (auto item = nameToItem.begin(); item != nameToItem.end(); item++)
						if (is->first.compare(item->first) == 0)
							item->second->setStock(item->second->getStock() + is->second);
				c->order.clear();
				return true;
			}
		}
	}
	return false;
}

// 支付
bool ESystem::pay(const string& username)
{
	double sum = 0;
	unsigned int stk = 0, sub = 0;
	string name, owner;
	for (auto it = nameToUser.begin(); it != nameToUser.end(); it++)
	{
		if (username.compare(it->first) == 0 && it->second->getUserType() == CUSTOMER)
		{
			Customer_ptr c = dynamic_pointer_cast<Customer> (it->second);
			if (c->order.size() == 0)
				return false;
			for (auto is = c->order.begin(); is != c->order.end(); is++)
			{
				sub = is->second;
				name = is->first;
				for (auto item = nameToItem.begin(); item != nameToItem.end(); item++)
				{
					if (name.compare(item->first) == 0)
					{
						stk = item->second->getStock() - sub;
						sum += item->second->getPrice() * sub;
					}
				}
			}

			// 确保用户支付成功后再增加商家账户中余额
			if (subBalance(username, sum))
			{
				for (auto is = c->order.begin(); is != c->order.end(); is++)
				{
					sub = is->second;
					name = is->first;
					for (auto item = nameToItem.begin(); item != nameToItem.end(); item++)
					{
						if (name.compare(item->first) == 0)
						{
							stk = item->second->getStock() - sub;
							owner = item->second->getOwner();
							item->second->setStock(stk);
							addBalance(owner, item->second->getPrice() * sub);
						}
					}
				}
				return true;
			}
		}
	}
	return false;
}

// 设置购物车内商品数目
bool ESystem::setItemQuantity(const string& name, const string& username, unsigned int num)
{
	for (auto it = nameToUser.begin(); it != nameToUser.end(); it++)
	{
		if (username.compare(it->first) == 0 && it->second->getUserType() == CUSTOMER)
		{
			for (auto is = (dynamic_pointer_cast<Customer>(it->second))->cart.begin(); is != (dynamic_pointer_cast<Customer>(it->second))->cart.end(); is++)
			{
				if (name.compare(is->first) == 0)
				{
					if (num > is->second) 
					{
						if (addToCart(name, username, num - is->second))
							return true;
					}
					else
					{
						if (removeFromCart(name, username, is->second - num))
							return true;
					}
				}
			}
		}
	}
	return false;
}