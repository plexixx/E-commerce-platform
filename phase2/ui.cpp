#include"ui.h"
#include"user.h"
#include"item.h"

using namespace std;

bool userInterface::login()
{
	system("cls");

	unsigned short int op;
	while (1)
	{
		try 
		{
			cout << "-----------------------------------\n"
				<< "1 -- ��¼\n2 -- ����\n"
				<< "-----------------------------------\n"
				<< "�����������:";
			cin >> op;
			if (cin.fail())throw invalid_argument("����������:");
			if (op == 0 || op > 2) throw invalid_argument("�Ƿ�������!\n����������:");
			if (op == 2)return false;

			string userName;
			string password;
			cout << "-----------------------------------\n"
				<<"�������û���������\n"
				<< "-----------------------------------\n"
				<<"�û���:";
			cin >> userName;
			cout << "����:";
			cin >> password;

			if (sys.checkPass(userName, password))
			{
				cout << "��¼�ɹ�!\n\n";
				ifLogin = true;
				username_t = userName;
				userType = sys.getUserType(username_t);
				return true;
			}
			else 
			{
				cout << "�û���/�������.\n";
				return false;
			}
		}
		catch(invalid_argument err)
		{
			if (!processError(err))break;
		}
	}
	return false;
}

void userInterface::displayInfo() const
{
	system("cls");
	sys.displayUserInfo(username_t);
	return;
}

bool userInterface::setPass()
{
	system("cls");
	string password;
	cout << "������������:";
	cin >> password;
	if (!cin.fail())
	{
		if (sys.setPass(username_t, password))
			return true;
	}
	return false;
}

bool userInterface::editItemInfo()
{
	system("cls");
	string name;
	cout << "��������޸ĵ���Ʒ����:";
	cin >> name;
	if (sys.ifHaveItem(name))
	{
		unsigned short int op = 0;
		cout << "-----------------------------------\n"
			<< setw(24) << "1 -- �޸���Ʒ����\n"
			<< setw(24) << "2 -- �޸���Ʒ���\n"
			<< setw(24) << "3 -- �޸���Ʒ�۸�\n"
			<< setw(24) << "4 -- ��Ʒ���۴���\n"
			<< setw(24) << "5 -- Ʒ����۴���\n"
			<< setw(24) << "6 -- �����ϼ��˵�\n"
			<< "-----------------------------------\n"
			<< "�����������:";
		cin >> op;
		string des;
		double ori = 0, dis = 1.0;
		unsigned int stk = 0;
		int type = sys.getItemType(name);
		switch (op)
		{
		case SET_DES:
			cout << "\n\n�������µ���Ʒ����:";
			cin >> des;
			if(!cin.fail())
				if (sys.setDescript(username_t, name, des))
					return true;
			break;
		case SET_STOCK:
			cout << "\n\n�������µ���Ʒ�����:";
			cin >> stk;
			if (!cin.fail())
			if (sys.setStock(username_t, name, stk))
			break;
				return true;
		case SET_PRICE:
			cout << "\n\n�������µ���Ʒ�۸�:";
			cin >> ori;
			if (!cin.fail())
			if (sys.setOrigin(username_t, name, ori))
				return true;
			break;
		case SET_DIS:
			cout << "\n\n��������Ʒ�ۿ�:";
			cin >> dis;
			if (!cin.fail())
			if (sys.setDiscount(username_t, name, dis))
				return true;
			break;
		case SET_TDIS:
			cout << "\n\n��������Ʒ�ۿ�:";
			cin >> dis;
			if (!cin.fail())
			if (sys.setDiscount(username_t, type, dis))
				return true;
			break;
		default:
			return false;
		}
	}
	else
		cout << "��Ʒ��Ϣ������!\n";
	return false;
}

bool userInterface::delUser()
{
	system("cls");
	char ch;
	cout << "��ȷ��Ҫע����? (y/n)";
	cin >> ch;
	switch (ch)
	{
	case 'y':
		if (sys.delUser(username_t))
			return true;
		break;
	default:
		break;
	}
	return false;
}

bool userInterface::recharge()
{
	system("cls");
	double b = 0;
	cout << "��ֵ���:";
	cin >> b;
	if (!cin.fail())
	if (sys.addBalance(username_t, b))
		return true;
	return false;
}

bool userInterface::addItem()
{
	system("cls");
	string name, descript;
	unsigned int stk;
	int type;
	double ori;
	cout << "��������Ʒ��Ϣ:\n";
	cout << "��Ʒ��:";
	cin >> name;
	cout << "\n��Ʒ����( 1 -- �鼮, 2 -- ��װ, 3 -- ʳƷ):";
	cin >> type;
	cout << "\n���:";
	cin >> stk;
	cout << "\n�۸�:";
	cin >> ori;
	cout << "\n��Ʒ����:";
	cin >> descript;
	if (!cin.fail())
		if (sys.addItem(name, username_t, descript, ori, stk, type))
			return true;
	return false;
}

bool userInterface::delItem()
{
	system("cls");
	string name;
	cout << "��������Ҫɾ������Ʒ����:";
	cin >> name;
	if (sys.delItem(username_t, name))
		return true;
	return false;
}

// -------------------------------------------------


// ע�����
bool userInterface::registerUser()
{
	system("cls");

	while (1)
	{
		try
		{
			cout << "-----------------------------------\n"
				<< "1 -- ��ͨ�û�ע��\n2 -- �̼�ע��\n3 -- ����\n"
				<< "-----------------------------------\n"
				<< "�����������:";
			cin >> userType;
			if (cin.fail())throw invalid_argument("����������:");
			if (userType > 3 || userType == 0)throw invalid_argument("�Ƿ�������!\n����������:");
			if (userType == 3)return false;

			string userName;
			string password;
			cout << "-----------------------------------\n"
				<< "�������û���������\n\n�û���:";
			cin >> userName;
			cout << "����:";
			cin >> password;

			if (sys.addUser(userName, password, userType))
				return true;
			return false;
		}
		catch (invalid_argument err)
		{
			if (!processError(err))break;
		}
	}
	return false;
}

// �鿴��Ʒ����
void userInterface::viewDetail(const string& name) const
{
	bool ret=sys.displayItemInfo(name);
	if (!ret)
		cout << "��Ʒ��Ϣ��ȡʧ��.\n";
	return;
}

// �鿴��Ʒ
void userInterface::viewItemList() const
{
	system("cls");
	sys.listItem();

	if (ifLogin && userType == CUSTOMER)
	{
		unsigned short int op = 0;
		string name;
		unsigned int num;
		cout << "-----------------------------------\n"
			<< "1 -- �����Ʒ�����ﳵ\n2 -- �鿴��Ʒ��ϸ��Ϣ\n3 -- ����\n"
			<< "-----------------------------------\n"
			<< "�����������:";
		cin >> op;
		switch (op)
		{
		case 1:
			cout << "-----------------------------------\n"
				<< "������Ҫ��ӵ���Ʒ��������:";
			cin >> name >> num;
			if (sys.addToCart(name, username_t, num))
				cout << "��ӳɹ�!\n";
			else
				cout << "���ʧ��.\n";
			break;
		case 2:
			cout << "-----------------------------------\n"
				<< "�������ѯ����Ʒ����:";
			cin >> name;
			viewDetail(name);
			break;
		default:
			break;
		}
	}
	return;
}

// ɸѡ��Ʒ
void userInterface::sortItemList() const
{
	int type = 0;
	system("cls");
	cout << "��ѡ����Ҫ�鿴����Ʒ����:\n"
		<< "1 -- �鼮\n2 -- ��װ\n3 -- ʳƷ\n\n"
		<< "�������������:";
	cin >> type;
	if (!cin.fail() && type > 0 && type < 4)
		sys.listItem(type);
	return;
}

// չʾ���ﳵ
void userInterface::viewCart() const
{
	system("cls");
	cout << "-----------------------------------\n"
		<< setw(20) << "���ﳵ\n"
		<< "-----------------------------------\n"
		<< setw(15) << "��Ʒ����"
		<< setw(15) << "����"
		<< endl;
	bool ret = sys.displayCart(username_t);
	if (ret == false)
		cout << "����տ���Ҳ(T_T)\n��ȥ��ѡ��Ʒ��\n";
	else
	{
		unsigned short int op = 0;
		unsigned int num = 0, n = 0;
		string str;
		vector<string> itemList;
		cout << "-----------------------------------\n"
			<< setw(20) << "1 -- �����µ�\n"
			<< setw(24) << "2 -- ������Ʒ����\n"
			<< setw(20) << "3 -- ɾ����Ʒ\n"
			<< setw(22) << "4 -- ��չ��ﳵ\n"
			//<< setw(20) << "5 -- �����ϼ��˵�\n"
			<< "-----------------------------------\n"
			<< "�����������:";
		cin >> op;
		switch (op)
		{
		case GEN_ORDER:
			cout << "�����빺�����Ʒ��:\n";
			while (cin >> str)
				itemList.push_back(str);
			cout << "\n����������...\n";
			for (auto it : itemList)
			{
				ret = sys.generateOrder(str, username_t);
				if (!ret)
				{
					cout << "��������ʧ��./(��o��)/~~\n";
					return;
				}
			}
			cout << "�������ɳɹ�!\n";
			break;
		case SET_QUANTITY:
			cout<< "����������ĵ���Ʒ�����޸ĺ�����:\n";
			cin >> str >> num;
			ret = sys.setItemQuantity(str, username_t, num);
			if (ret)
				cout << "�޸ĳɹ�!\n";
			else
				cout << "�޸�ʧ��.\n";
			break;
		case DEL_CART:
			cout << "�������ɾ������Ʒ��:\n";
			while (cin >> str)
				itemList.push_back(str);
			for (auto it : itemList)
			{
				ret = sys.removeFromCart(str, username_t);
				if (!ret)
				{
					cout << "ɾ��ʧ��./(��o��)/~~\n";
					return;
				}
			}
			cout << "ɾ���ɹ�!\n";
			break;
		case CLR_CART:
			if (sys.clearCart(username_t))
				cout << "��ճɹ�!\n";
			else
				cout << "���ʧ��.\n";
			break;
		default:
			break;
		}
	}
	system("pause");
	return;
}

// չʾ����
void userInterface::viewOrder() const
{
	unsigned short int op = 0;
	system("cls");
	cout << "-----------------------------------\n"
		<< setw(20) << "�����б�\n"
		<< "-----------------------------------\n"
		<< setw(20) << "��Ʒ����"
		<< setw(20) << "����"
		<< endl;
	double sum = sys.displayOrder(username_t);
	if (sum == 0)
		cout << "����տ���Ҳ(T_T)\n��ȥ��ѡ��Ʒ��\n";
	cout << "-----------------------------------\n"
		<< "�ܽ��:" << sum << endl
		<< "1 -- ����֧��\n2 -- ɾ������\n3 -- ����\n�����������:";
	cin >> op;
	switch (op)
	{
	case 1:
		if (sys.pay(username_t))
			cout << "֧���ɹ�!\n";
		else
			cout << "֧��ʧ��.\n";
		break;
	case 2:
		if(sys.delOrder(username_t))
			cout << "ɾ���ɹ�!\n";
		else
			cout << "ɾ��ʧ��.\n";
	default:
		break;
	}
	system("pause");
	return;
}

// ��ͨ�û�����
void userInterface::crInterface()
{
	unsigned short int op;
	while (1)
	{
		try
		{
			system("cls");
			cout << "-----------------------------------\n"
				<< setw(24) << "3 -- ������Ϣ\n"
				<< setw(24) << "4 -- ��������\n"
				<< setw(24) << "5 -- ����ֵ\n"
				<< setw(22) << "6 -- ���ﳵ\n"
				<< setw(24) << "7 -- ��������\n"
				<< setw(28) << "9 -- �鿴ȫ����Ʒ\n"
				<< setw(30) << "10 -- ������ɸѡ��Ʒ\n"
				<< setw(24) << "11 -- �˳���¼\n"
				<< setw(24) << "12 -- ע���˺�\n"
				//<< setw(20) << "14 -- �˳�\n"
				<< "-----------------------------------\n"
				<< "�����������:";
			cin >> op;
			if (cin.fail())throw invalid_argument("����������:");
			if (op < 3 || op > 12 || op == 8)
				throw invalid_argument("�Ƿ�������!\n����������:");
			//if (op == 14)show();
		}
		catch (invalid_argument err)
		{
			if (!processError(err))break;
		}
		switch (op)
		{
		case USER_INFO:
			displayInfo();
			system("pause");
			break;
		case RESET_PW:
			if (setPass())
				cout << "�������óɹ�!\n";
			else
				cout << "����ʧ��.\n";
			system("pause");
			break;
		case RECHARGE:
			if (recharge())
				cout << "��ֵ�ɹ�!\n";
			else
				cout << "��ֵʧ��.\n";
			system("pause");
			break;
		case CART:
			viewCart();
			system("pause");
			break;
		case ORDER_LIST:
			viewOrder();
			system("pause");
			break;
		case LIST_ALL_ITEM:
			viewItemList();
			system("pause");
			break;
		case SORT_ITEM:
			sortItemList();
			system("pause");
			break;
		case LOGOUT:
			ifLogin = false;
			userType = 0;
			sys.saveItemFile();
			sys.saveUserFile();
			system("pause");
			break;
		case DEACTIVE:
			if (delUser())
			{
				cout << "ע���ɹ�!\n";
				ifLogin = false;
				sys.saveItemFile();
				sys.saveUserFile();
			}
			else
				cout << "ע��ʧ��.\n";
			userType = 0;
			system("pause");
			break;
		}
		if (!ifLogin)show();
		if (userType == 1 && ifLogin)crInterface();
		if (userType == 2 && ifLogin)trInterface();
	}
}

// �̼ҽ���
void userInterface::trInterface()
{
	unsigned short int op;
	while (1)
	{
		try
		{
			system("cls");
			cout << "-----------------------------------\n"
				<< setw(24) << "3 -- ������Ϣ\n"
				<< setw(24) << "4 -- ��������\n"
				<< setw(24) << "5 -- ����ֵ\n"
				<< setw(24) << "6 -- �����Ʒ\n"
				<< setw(28) << "7 -- �޸���Ʒ��Ϣ\n"
				<< setw(24) << "8 -- ɾ����Ʒ\n"
				<< setw(28) << "9 -- �鿴ȫ����Ʒ\n"
				<< setw(30) << "10 -- ������ɸѡ��Ʒ\n"
				<< setw(24) << "11 -- �˳���¼\n"
				<< setw(24) << "12 -- ע���˺�\n"
				//<< setw(20) << "14 -- �˳�\n"
				<< "-----------------------------------\n"
				<< "�����������:";
			cin >> op;
			if (cin.fail())throw invalid_argument("����������:");
			if (op < 3 || op > 14 || op == 13)
				throw invalid_argument("�Ƿ�������!\n����������:");
			if (op == 14)return;
		}
		catch (invalid_argument err)
		{
			if (!processError(err))break;
		}
		switch (op)
		{
		case USER_INFO:
			displayInfo();
			system("pause");
			break;
		case RESET_PW:
			if (setPass())
				cout << "�������óɹ�!\n";
			else
				cout << "����ʧ��.\n";
			system("pause");
			break;
		case RECHARGE:
			if (recharge())
				cout << "��ֵ�ɹ�!\n";
			else
				cout << "��ֵʧ��.\n";
			system("pause");
			break;
		case ADD_ITEM:
			if(addItem())
				cout << "��ӳɹ�!\n";
			else
				cout << "���ʧ��.\n";
			system("pause");
			break;
		case EDIT_ITEM_INFO:
			if(editItemInfo())
				cout << "�޸ĳɹ�!\n";
			else
				cout << "�޸�ʧ��.\n";
			system("pause");
			break;
		case DEL_ITEM:
			if(delItem())
				cout << "ɾ���ɹ�!\n";
			else
				cout << "ɾ��ʧ��.\n";
			system("pause");
			break;
		case LIST_ALL_ITEM:
			viewItemList();
			system("pause");
			break;
		case SORT_ITEM:
			sortItemList();
			system("pause");
			break;
		case LOGOUT:
			ifLogin = false;
			userType = 0;
			sys.saveItemFile();
			sys.saveUserFile();
			break;
		case DEACTIVE:
			if (delUser())
			{
				cout << "ע���ɹ�!\n";
				ifLogin = false;
				sys.saveItemFile();
				sys.saveUserFile();
			}
			else
				cout << "ע��ʧ��.\n";
			userType = 0;
			system("pause");
			break;
		}
		if (!ifLogin)show();
		if (userType == 1 && ifLogin)crInterface();
		if (userType == 2 && ifLogin)trInterface();
	}
}



// ��ʼ����
void userInterface::show()
{
	unsigned short int op=0;
	while (op!=EXIT)
	{
		try
		{
			system("cls");
			cout << "-----------------------------------\n"
				<< "       ��ӭʹ��xxx���̽���ƽ̨       \n"
				<< "-----------------------------------\n"
				<< setw(24) << "1 -- �û���¼\n"
				<< setw(24) << "2 -- ע���˺�\n"
				<< setw(24) << "9 -- �鿴��Ʒ\n"
				<< setw(24) << "14 -- �˳�����\n"
				<< "-----------------------------------\n"
				<< "�����������:";
			cin >> op;
			if (cin.fail())throw invalid_argument("����������:");
			if (op > 2 && op != 9 && op != 14)throw invalid_argument("�Ƿ�������!\n����������:");
			//if (op == 14)return ;
		}
		catch (invalid_argument err)
		{
			if (!processError(err))break;
		}
		switch (op)
		{
		case LOGIN:
			if (login())
				ifLogin = true;
			else
				std::cerr << "��¼ʧ��!\n";
			system("pause");
			break;
		case REG:
			if (registerUser())
				cout << "ע��ɹ�!\n\n";
			else
				cout << "ע��ʧ��.\n\n";
			userType = 0;
			system("pause");
			break;
		case LIST_ALL_ITEM:
			viewItemList();
			system("pause");
			break;
		case EXIT:
			sys.saveItemFile();
			sys.saveUserFile();
			break;
		}
		if (userType == 1 && ifLogin)crInterface();
		if (userType == 2 && ifLogin)trInterface();
	}
	exit(0);
}

userInterface::userInterface()
{
	userInterface::sys = ESystem::getInstance();
	sys.readItemFile();
	sys.readUserFile();
}
