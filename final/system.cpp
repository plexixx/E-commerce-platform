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

// ��ȡtokenToUser��username��Ӧtokenֵ
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

// ���з�����
void ESystem::run(const string& port)
{
	char buffRecv[MAX_BUF];
	char buffSend[MAX_BUF];

	cout << "���ڼ���:" << port << "�˿�\n";

	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		cout << "WSAStartup����ʧ��.\n";
		return;
	}

	SOCKET serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in myAddr;
	myAddr.sin_family = AF_INET;
	myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myAddr.sin_port = htons(atoi(port.c_str()));

	if (bind(serverSock, (sockaddr*)&myAddr, sizeof(myAddr)) != 0)
	{
		cout << "��ʧ��.\n";
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
		cout << "��������.\n";

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
		// ��¼
		case 1:
			iss >> username >> pass;
			cout << username << endl;
			cout << pass << endl;

			// �û��������벻ƥ�䣬����0
			if (!checkPass(username, pass))
			{
				buffSend[0] = '0';
				len++;
			}
			else
			{
				token = getValue(username);
				// δ�ɹ���ȡtokenֵ������1
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
			
					// ���ĸ�ʽ
					// [2 token �û����� padding]
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

		// ע��
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

		// �鿴�û���Ϣ
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

				// ���ĸ�ʽ
				// [1 �û����� ������� padding]
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

		// ��������
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

		// ��ֵ
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

		// �����Ʒ
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

		// �鿴��Ʒ��ϸ��Ϣ
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

				// ���ĸ�ʽ
				// [1 �̼��� ԭ�� �ּ� ��� ��Ʒ��ϸ���� padding]
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

		// ɾ����Ʒ
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

		// �鿴ȫ����Ʒ
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

					// ���ĸ�ʽ
					// [1 ��Ʒ�� �ּ� ��Ʒ���� ��� padding]
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

		// ɸѡ��Ʒ
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

						// ���ĸ�ʽ
						// [1 ��Ʒ�� �ּ� ��Ʒ���� ��� padding]
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

		// �˳���¼
		case 11:
			iss >> tk;
			token = atoi(tk.c_str());
			tokenToUser.erase(token);
			buffSend[0] = '1';
			len++;
			saveItemFile();
			saveUserFile();
			break; 

		// ע���˺�
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

		// �˳�
		case 14:
			saveItemFile();
			saveUserFile();
			break;

		// �鿴���ﳵ 
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

							// ���ĸ�ʽ
							// [1 ��Ʒ�� ���� padding]
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

		// �鿴�����б�
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

				// ���ĸ�ʽ
				// [1 ��Ʒ�� ���� padding]
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

		// �޸���Ʒ����
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

		// �޸���Ʒ���
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

		// �޸���Ʒԭ��
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

		// �޸ĵ�Ʒ�ۿ�
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

		// �޸���Ʒ���ۿ�
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

		// ���ɶ���
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

		// �޸Ĺ��ﳵ����Ʒ����
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

		// ɾ�����ﳵ����Ʒ
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

		// ��չ��ﳵ
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

		// ɾ������
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

		// �����Ʒ�����ﳵ
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

		// ֧��
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


// ------------- ���ݴ�ȡ ------------- 

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

// ��ȡ���
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

// ��ȡ��Ʒԭ��
double ESystem::getOrigin(const string& name) const
{
	for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
	{
		if (name.compare(it->first) == 0)
			return it->second->getOrigin();
	}
	return 0;
}

// ��ȡ��Ʒ����
string ESystem::getDescript(const string& name) const
{
	for (auto it = nameToItem.begin(); it != nameToItem.end(); it++)
	{
		if (name.compare(it->first) == 0)
			return it->second->getDescript();
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
				
				// ������Ʒ��Ӧ���
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