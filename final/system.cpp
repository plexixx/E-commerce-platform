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

// 获取tokenToUser中username对应token值
int ESystem::getValue(const string& username) 
{
	int ret = -1;

	for (auto it : tokenToUser)
	{
		if (it.second == username)
		{
			return -1;
		}
	}

	if (tokenToUser.size() == 128)
	{
		return ret;
	}
	for (int i = 1; i < 128; i++)
	{
		auto it = tokenToUser.find(i);
		if (it == tokenToUser.end())
		{
			ret = i;
			tokenToUser[i] = username;
			break;
		}
	}
	return ret;
}

// 运行服务器
void ESystem::run(const string& port)
{
	char buffRecv[MAX_BUF];
	char buffSend[MAX_BUF];

	cout << "正在监听:" << port << "端口\n";

	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		cout << "WSAStartup调用失败.\n";
		return;
	}

	SOCKET serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in myAddr;
	myAddr.sin_family = AF_INET;
	myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myAddr.sin_port = htons(atoi(port.c_str()));

	if (bind(serverSock, (sockaddr*)&myAddr, sizeof(myAddr)) != 0)
	{
		cout << "绑定失败.\n";
		return;
	}

	listen(serverSock, 5);

	SOCKET clientSock;
	sockaddr_in clientAddr;
	int addrLen = sizeof(clientAddr);

	//char op = '\0';
	unsigned short op =0;

	unsigned int len = 0;

	while (op != EXIT)
	{
		memset(buffRecv, 0, MAX_BUF);
		memset(buffSend, 0, MAX_BUF);
		memset(&clientAddr, 0, sizeof(clientAddr));
		len = 0;

		clientSock=accept(serverSock, (sockaddr*)&clientAddr, (socklen_t*)&addrLen);
		cout << "接受请求.\n";

		int ret = recv(clientSock, buffRecv, MAX_BUF, 0);

		cout << "len : " << ret << endl;

		istringstream iss(buffRecv);
		iss >> op;

		cout << "op : " << op << endl;
		cout << (unsigned short)op << endl;

		string username, pass, name, tk, descript;
		unsigned int type, stk, userType;
		int token = -1;
		int num;
		double mon, sum;
		
		switch (op)
		{
		// 登录
		case 1:
			iss >> username >> pass;
			cout << username << endl;
			cout << pass << endl;

			// 用户名和密码不匹配，发送0
			if (!checkPass(username, pass))
			{
				buffSend[0] = '0';
				len++;
			}
			else
			{
				token = getValue(username);
				// 未成功获取token值，发送1
				if (token == -1)
				{
					buffSend[0] = '1';
					len++;
				}
				else
				{
					buffSend[0] = '2';
					buffSend[1] = ' ';
					len += 2;
					
					userType = getUserType(username);
			
					// 报文格式
					// [2 token 用户类型 padding]
					string str;
					str = to_string(token);
					//cout << "token:" << str << endl;
					memcpy(buffSend + len, str.c_str(), strlen(str.c_str()));
					len += strlen(str.c_str());
					buffSend[len] = ' ';
					len++;
					str = to_string(userType);
					//cout << "usertype:" << str << endl;
					memcpy(buffSend + len, str.c_str(), strlen(str.c_str()));
					len += strlen(str.c_str());
					
					len += MAX_BUF;
				}
			}
			cout << token << endl;
			break;

		// 注册
		case 2:
			iss >> username >> pass >> userType;
			if (addUser(username, pass, userType))
			{
				buffSend[0] = '1';
				len++;
			}
			else
			{
				buffSend[0] = '0';
				len++;
			}
			break; 

		// 查看用户信息
		case 3:
			iss >> tk;
			token = atoi(tk.c_str());
			username = tokenToUser[token];
			if (!displayUserInfo(username))
			{
				buffSend[0] = '0';
				len++;
			}
			else
			{
				buffSend[0] = '1';
				buffSend[1] = ' ';
				len += 2;
				double bal = getBalance(username);
				unsigned int tp = getUserType(username);

				// 报文格式
				// [1 用户类型 余额类型 padding]
				string str;
				str = to_string(tp);
				memcpy(buffSend + len, str.c_str(), strlen(str.c_str()));
				len += strlen(str.c_str());
				buffSend[len] = ' ';
				len++;
				str = to_string(bal);
				memcpy(buffSend + len, str.c_str(), strlen(str.c_str()));
				len += strlen(str.c_str());
			}
			break;

		// 重置密码
		case 4:
			iss >> tk >> pass;
			token = atoi(tk.c_str());
			username = tokenToUser[token];
			if(!setPass(username,pass))
			{
				buffSend[0] = '0';
				len++;
			}
			else
			{
				buffSend[0] = '1';
				len++;
			}
			break; 

		// 充值
		case 5:
			iss >> tk >> mon;
			token = atoi(tk.c_str());
			username = tokenToUser[token];
			if (!addBalance(username, mon))
			{
				buffSend[0] = '0';
				len++;
			}
			else
			{
				buffSend[0] = '1';
				len++;
			}
			break; 

		// 添加商品
		case 6:
			iss >> name >> tk >> descript >> mon >> stk >> type;
			token = atoi(tk.c_str());
			username = tokenToUser[token];
			if (!addItem(name, username, descript, mon, stk, type))
			{
				buffSend[0] = '0';
				len++;
			}
			else
			{
				buffSend[0] = '1';
				len++;
			}
			break; 

		// 查看商品详细信息
		case 7:
			iss >> tk >> name;
			token = atoi(tk.c_str());
			username = tokenToUser[token];
			if (!displayItemInfo(name))
			{
				buffSend[0] = '0';
				len++;
			}
			else
			{
				buffSend[0] = '1';
				buffSend[1] = ' ';
				len += 2;
				string owner = getOwner(name);
				double ori = getOrigin(name);
				double pri = getPrice(name);
				unsigned int stock = getStock(name);
				string des = getDescript(name);

				// 报文格式
				// [1 商家名 原价 现价 库存 商品详细描述 padding]
				string str;
				str = owner;
				memcpy(buffSend + len, str.c_str(), strlen(str.c_str()));
				len += strlen(str.c_str());
				buffSend[len] = ' ';
				len++;
				str = to_string(ori);
				memcpy(buffSend + len, str.c_str(), strlen(str.c_str()));
				len += strlen(str.c_str());
				buffSend[len] = ' ';
				len++;
				str = to_string(pri);
				memcpy(buffSend + len, str.c_str(), strlen(str.c_str()));
				len += strlen(str.c_str());
				buffSend[len] = ' ';
				len++;
				str = to_string(stock);
				memcpy(buffSend + len, str.c_str(), strlen(str.c_str()));
				len += strlen(str.c_str());
				buffSend[len] = ' ';
				len++;
				str = des;
				memcpy(buffSend + len, str.c_str(), strlen(str.c_str()));
				len += strlen(str.c_str());
			}
			break;

		// 删除商品
		case 8:
			iss >> tk >> name;
			token = atoi(tk.c_str());
			username = tokenToUser[token];
			if (!delItem(username, name))
			{
				buffSend[0] = '0';
				len++;
			}
			else
			{
				buffSend[0] = '1';
				len++;
			}
			break; 

		// 查看全部商品
		case 9:
			if (nameToItem.size() == 0)
			{
				buffSend[0] = '0';
				len++;
			}
			else
			{
				buffSend[0] = '1';
				buffSend[1] = ' ';
				len += 2;
				string iname;
				double pri;
				unsigned int tp, stock;
				for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
				{
					if (it != nameToItem.begin()) {
						buffSend[len] = ' ';
						len++;
					}
					iname = it->first;
					pri = it->second->getPrice();
					tp = it->second->getItemType();
					stock = it->second->getStock();

					// 报文格式
					// [1 商品名 现价 商品类型 库存 padding]
					string str;
					str = iname;
					memcpy(buffSend + len, str.c_str(), strlen(str.c_str()));
					len += strlen(str.c_str());
					buffSend[len] = ' ';
					len++;
					str = to_string(pri);
					memcpy(buffSend + len, str.c_str(), strlen(str.c_str()));
					len += strlen(str.c_str());
					buffSend[len] = ' ';
					len++;
					str = to_string(tp);
					memcpy(buffSend + len, str.c_str(), strlen(str.c_str()));
					len += strlen(str.c_str());
					buffSend[len] = ' ';
					len++;
					str = to_string(stock);
					memcpy(buffSend + len, str.c_str(), strlen(str.c_str()));
					len += strlen(str.c_str());
				}
			}
			break; 

		// 筛选商品
		case 10:
			iss >> type;
			if (nameToItem.size() == 0)
			{
				buffSend[0] = '0';
				len++;
			}
			else
			{
				buffSend[0] = '1';
				buffSend[1] = ' ';
				len += 2;
				string iname;
				double pri;
				unsigned int tp, stock;

				for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
				{
					if (it->second->getItemType() == type)
					{
						if (it != nameToItem.begin()) {
							buffSend[len] = ' ';
							len++;
						}
						iname = it->first;
						pri = it->second->getPrice();
						tp = it->second->getItemType();
						stock = it->second->getStock();

						// 报文格式
						// [1 商品名 现价 商品类型 库存 padding]
						string str;
						str = iname;
						memcpy(buffSend + len, str.c_str(), strlen(str.c_str()));
						len += strlen(str.c_str());
						buffSend[len] = ' ';
						len++;
						str = to_string(pri);
						memcpy(buffSend + len, str.c_str(), strlen(str.c_str()));
						len += strlen(str.c_str());
						buffSend[len] = ' ';
						len++;
						str = to_string(tp);
						memcpy(buffSend + len, str.c_str(), strlen(str.c_str()));
						len += strlen(str.c_str());
						buffSend[len] = ' ';
						len++;
						str = to_string(stock);
						memcpy(buffSend + len, str.c_str(), strlen(str.c_str()));
						len += strlen(str.c_str());
					}
				}
			}
			break; 

		// 退出登录
		case 11:
			iss >> tk;
			token = atoi(tk.c_str());
			tokenToUser.erase(token);
			buffSend[0] = '1';
			len++;
			saveItemFile();
			saveUserFile();
			break; 

		// 注销账号
		case 12:
			iss >> tk;
			token = atoi(tk.c_str());
			username = tokenToUser[token];
			if (!delUser(username))
			{
				buffSend[0] = '0';
				len++;
			}
			else
			{
				buffSend[0] = '1';
				tokenToUser.erase(token);
				len++;
			}
			break; 

		// 退出
		case 14:
			saveItemFile();
			saveUserFile();
			break;

		// 查看购物车 
		case 15:
			iss >> tk;
			token = atoi(tk.c_str());
			username = tokenToUser[token];
			if (!displayCart(username))
			{
				buffSend[0] = '0';
				len++;
			}
			else
			{
				buffSend[0] = '1';
				buffSend[1] = ' ';
				len += 2;
				string item;
				unsigned int n;
				for (auto it = nameToUser.begin(); it != nameToUser.end(); it++)
				{
					if (username.compare(it->first) == 0 && it->second->getUserType() == CUSTOMER)
					{
						for (auto is = (dynamic_pointer_cast<Customer>(it->second))->cart.begin(); is != (dynamic_pointer_cast<Customer>(it->second))->cart.end(); is++)
						{
							if (is != (dynamic_pointer_cast<Customer>(it->second))->cart.begin()){
								buffSend[len] = ' ';
								len++;
							}
							item = is->first;
							n = is->second;

							// 报文格式
							// [1 商品名 数量 padding]
							string str;
							str = item;
							memcpy(buffSend + len, str.c_str(), strlen(str.c_str()));
							len += strlen(str.c_str());
							buffSend[len] = ' ';
							len++;
							str = to_string(n);
							memcpy(buffSend + len, str.c_str(), strlen(str.c_str()));
							len += strlen(str.c_str());
						}
					}
				}
			}
			break; 

		// 查看订单列表
		case 16:
			iss >> tk;
			token = atoi(tk.c_str());
			username = tokenToUser[token];
			sum = displayOrder(username);
			if (sum == 0)
			{
				buffSend[0] = '0';
				len++;
			}
			else
			{
				buffSend[0] = '1';
				buffSend[1] = ' ';
				len += 2;
				string item;
				unsigned int n;

				// 报文格式
				// [1 商品名 数量 padding]
				string str;
				str = to_string(sum);
				memcpy(buffSend + len, str.c_str(), strlen(str.c_str()));
				len += strlen(str.c_str());
				buffSend[len] = ' ';
				len++;
				for (auto it = nameToUser.begin(); it != nameToUser.end(); it++)
				{
					if (username.compare(it->first) == 0 && it->second->getUserType() == CUSTOMER)
					{
						for (auto is = (dynamic_pointer_cast<Customer>(it->second))->order.begin(); is != (dynamic_pointer_cast<Customer>(it->second))->order.end(); is++)
						{
							if (is != (dynamic_pointer_cast<Customer>(it->second))->order.begin()) {
								buffSend[len] = ' ';
								len++;
							}
							item = is->first;
							n = is->second;

							str = item;
							memcpy(buffSend + len, str.c_str(), strlen(str.c_str()));
							len += strlen(str.c_str());
							buffSend[len] = ' ';
							len++;
							str = to_string(n);
							memcpy(buffSend + len, str.c_str(), strlen(str.c_str()));
							len += strlen(str.c_str());
						}
					}
				}
			}
			break; 

		// 修改商品描述
		case 17:
			iss >> tk >> name >> descript;
			token = atoi(tk.c_str());
			username = tokenToUser[token];
			if (!setDescript(username,name,descript))
			{
				buffSend[0] = '0';
				len++;
			}
			else
			{
				buffSend[0] = '1';
				len++;
			}
			break; 

		// 修改商品库存
		case 18:
			iss >> tk >> name >> stk;
			token = atoi(tk.c_str());
			username = tokenToUser[token];
			if (!setStock(username, name, stk))
			{
				buffSend[0] = '0';
				len++;
			}
			else
			{
				buffSend[0] = '1';
				len++;
			}
			break; 

		// 修改商品原价
		case 19:
			iss >> tk >> name >> mon;
			token = atoi(tk.c_str());
			username = tokenToUser[token];
			if (!setOrigin(username, name, mon))
			{
				buffSend[0] = '0';
				len++;
			}
			else
			{
				buffSend[0] = '1';
				len++;
			}
			break; 

		// 修改单品折扣
		case 20:
			iss >> tk >> name >> mon;
			token = atoi(tk.c_str());
			username = tokenToUser[token];
			if (!setDiscount(username, name, mon))
			{
				buffSend[0] = '0';
				len++;
			}
			else
			{
				buffSend[0] = '1';
				len++;
			}
			break; 

		// 修改商品类折扣
		case 21:
			iss >> tk >> type >> mon;
			token = atoi(tk.c_str());
			username = tokenToUser[token];
			if (!setDiscount(username, type, mon))
			{
				buffSend[0] = '0';
				len++;
			}
			else
			{
				buffSend[0] = '1';
				len++;
			}
			break; 

		// 生成订单
		case 22:
			iss >> tk >> name;
			token = atoi(tk.c_str());
			username = tokenToUser[token];
			if(!generateOrder(name,username))
			{
				buffSend[0] = '0';
				len++;
			}
			else
			{
				buffSend[0] = '1';
				len++;
			}
			break; 

		// 修改购物车内商品数量
		case 23:
			iss >> tk >> name >> num;
			token = atoi(tk.c_str());
			username = tokenToUser[token];
			if (!setItemQuantity(name, username, num))
			{
				buffSend[0] = '0';
				len++;
			}
			else
			{
				buffSend[0] = '1';
				len++;
			}
			break; 

		// 删除购物车内商品
		case 24:
			iss >> tk >> name;
			token = atoi(tk.c_str());
			username = tokenToUser[token];
			if (!removeFromCart(name, username))
			{
				buffSend[0] = '0';
				len++;
			}
			else
			{
				buffSend[0] = '1';
				len++;
			}
			break;

		// 清空购物车
		case 25:
			iss >> tk;
			token = atoi(tk.c_str());
			username = tokenToUser[token];
			if (!clearCart(username))
			{
				buffSend[0] = '0';
				len++;
			}
			else
			{
				buffSend[0] = '1';
				len++;
			}
			break;

		// 删除订单
		case 26:
			iss >> tk;
			token = atoi(tk.c_str());
			username = tokenToUser[token];
			if(!delOrder(username))
			{
				buffSend[0] = '0';
				len++;
			}
			else
			{
				buffSend[0] = '1';
				len++;
			}
			break;

		// 添加商品至购物车
		case 27:
			iss >> tk >> name >> num;
			token = atoi(tk.c_str());
			username = tokenToUser[token];
			if (!ifHaveItem(name)|| !addToCart(name, username, num))
			{
				buffSend[0] = '0';
				len++;
			}
			else
			{
				buffSend[0] = '1';
				len++;
			}
			break;

		// 支付
		case 28:
			iss >> tk;
			token = atoi(tk.c_str());
			username = tokenToUser[token];
			if (!pay(username))
			{
				buffSend[0] = '0';
				len++;
			}
			else
			{
				buffSend[0] = '1';
				len++;
			}
			break;
		default:
			break;
		}

		int a = send(clientSock, buffSend, len, 0);
		cout << len << endl;
		cout << a << endl;
		closesocket(clientSock);

		saveItemFile();
		saveUserFile();
	}
	WSACleanup();
}


// ------------- 数据存取 ------------- 

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

// 获取余额
double ESystem::getBalance(const string& username) const
{
	double b = 0;
	for (auto it = nameToUser.begin(); it != nameToUser.end(); it++)
	{
		if (username.compare(it->first) == 0)
		{
			b = it->second->getBalance();
		}
	}
	return b;
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

// 获取商品原价
double ESystem::getOrigin(const string& name) const
{
	for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
	{
		if (name.compare(it->first) == 0)
			return it->second->getOrigin();
	}
	return 0;
}

// 获取商品描述
string ESystem::getDescript(const string& name) const
{
	for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
	{
		if (name.compare(it->first) == 0)
			return it->second->getDescript();
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
				
				// 冻结商品对应库存
				if (c->generateOrder(name, getStock(name)))
				{
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