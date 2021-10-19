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

// ------------- ���ݴ�ȡ ------------- 

// ��ȡ�û���Ϣ
bool ESystem::readUserFile(const string& fp)
{
	ifstream ifs(fp, ios::in);
	if (!ifs.good())
	{
		cout << "���ݶ�ȡʧ��!\n";
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
	cout << "�û����ݶ�ȡ�ɹ�.\n";
	system("pause");
	return true;
}

// �洢�û���Ϣ
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
	cout << "�û����ݴ洢�ɹ�.\n";
	system("pause");
	return true;
}

// ��ȡ��Ʒ��Ϣ
bool ESystem::readItemFile(const string& fp)
{
	ifstream ifs(fp, ios::in);
	if (!ifs.good())
	{
		cout << "���ݶ�ȡʧ��!\n";
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
	cout << "��Ʒ���ݶ�ȡ�ɹ�.\n";
	system("pause");
	return true;
}

// �洢��Ʒ��Ϣ
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
	cout << "��Ʒ���ݴ洢�ɹ�.\n";
	system("pause");
	return true;
}


// ------------- �û����� ------------- 

// ����û�
bool ESystem::addUser(const string& username, const string& password, unsigned int userType)
{
	if (ifHaveUser(username)) 
	{
		cout << "�û�����ע��!\n";
		return false;
	}

	// �������Ϸ���
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

// ɾ���û�
bool ESystem::delUser(const string& username)
{
	for (auto it = nameToUser.begin(); it != nameToUser.end(); it++)
	{
		if (username.compare("admin") == 0)
		{
			cout << "����Ա�˺Ų���ɾ��!\n";
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

// �ж��û��Ƿ����
bool ESystem::ifHaveUser(const string& username) const
{
	for (auto it = nameToUser.begin(); it != nameToUser.end(); it++)
	{
		if (username.compare(it->first) == 0)
			return true;
	}
	return false;
}

// չʾ�û�������Ϣ
bool ESystem::displayUserInfo(const string& username) const
{
	if (ifHaveUser(username))
	{
		for (auto it = nameToUser.begin(); it != nameToUser.end(); it++)
		{
			if (username.compare(it->first) == 0)
			{
				cout << "�û���Ϣ:\n�û���:" << it->first
					<< "\n�û�����:" << it->second->getUserType()
					<< "\t( 1 -- ��ͨ�û�, 2 -- �̼� )"
					<< "\n�˻����:" << it->second->getBalance() << endl;
				return true;
			}
		}
	}
	return false;
}

// �������
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

// ������
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

// ��¼���
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

// ��������
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

// ��ȡ�û�����
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


// ------------- ��Ʒ���� ------------- 

// �����Ʒ
bool ESystem::addItem(const string& name, const string& username, const string& des, double origin, unsigned int stock, int type)
{
	for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
	{
		if (name.compare(it->first) == 0 && username.compare(it->second->getOwner()))
		{
			cout << "��Ʒ�Ѵ���!\n";
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

// ɾ����Ʒ
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

// ɾ���̼�������Ʒ
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

// �ж���Ʒ�Ƿ����
bool ESystem::ifHaveItem(const string& name) const
{
	for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
	{
		if (name.compare(it->first) == 0)
			return true;
	}
	return false;
}

// չʾ������Ʒ
void ESystem::listItem() const
{
	cout << setw(20) << "��Ʒ����"
		<< setw(20) << "�۸�"
		<< setw(20) << "����"
		<< setw(20) << "���" 
		<< setw(20) << "�̼�"
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

// չʾָ��Ʒ����Ʒ
void ESystem::listItem(int type) const
{
	cout << setw(25) << "��Ʒ����"
		<< setw(25) << "�۸�"
		<< setw(25) << "����"
		<< setw(25) << "���" << endl;
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

// չʾ��Ʒ��ϸ��Ϣ
bool ESystem::displayItemInfo(const string& name) const
{
	for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
	{
		if (name.compare(it->first) == 0)
		{
			int type = it->second->getItemType();
			double price = it->second->getPrice();
			cout << "��Ʒ��Ϣ:\n��Ʒ����:" << it->first
				<< "\n�̼�:" << it->second->getOwner()
				<< "\nԭ��:" << it->second->getOrigin()
				<< "\n�ּ�:" << price
				<< "\n���:" << it->second->getStock()
				<< "\n��Ʒ����:" << it->second->getDescript()
				<< endl;
			return true;
		}
	}
	return false;
}

// ������Ʒԭ��
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

// ������Ʒ�ۿ�
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

// ����Ʒ���ۿ�
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

// ������Ʒ���
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

// ������Ʒ����
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

// ��ȡ��Ʒ����
int ESystem::getItemType(const string& name) const
{
	for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
	{
		if (name.compare(it->first) == 0)
			return it->second->getItemType();
	}
	return 0;
}

// ��ȡ��Ʒ�۸�
double ESystem::getPrice(const string& name) const
{
	for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
	{
		if (name.compare(it->first) == 0)
			return it->second->getPrice();
	}
	return 0;
}

// ��ȡ��Ʒ�̼�
string ESystem::getOwner(const string& name) const
{
	for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
	{
		if (name.compare(it->first) == 0)
			return it->second->getOwner();
	}
	return 0;
}

// ��ȡ��Ʒ���
unsigned int ESystem::getStock(const string& name) const
{
	for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
	{
		if (name.compare(it->first) == 0)
			return it->second->getStock();
	}
	return 0;
}



// ------------- ���� ------------- 

// չʾ���ﳵ
bool ESystem::displayCart(const string& username) const
{
	if (ifHaveUser(username))
	{
		for (auto it = nameToUser.begin(); it != nameToUser.end(); it++)
		{
			if (username.compare(it->first) == 0 && it->second->getUserType() == CUSTOMER)
			{
				// ���ﳵΪ��
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

// չʾ����
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

// �����Ʒ�����ﳵ
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

// �Ƴ����ﳵ����Ʒ
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

// �Ƴ����ﳵ��һ����Ŀ����Ʒ
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

// ��չ��ﳵ
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

// ���ɶ���
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
					// ������Ʒ��Ӧ���
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

// ɾ������
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

// ֧��
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

			// ȷ���û�֧���ɹ����������̼��˻������
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

// ���ù��ﳵ����Ʒ��Ŀ
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