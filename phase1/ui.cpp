#include"ui.h"
#include"user.h"
#include"item.h"

using namespace std;

// 登录界面
bool userInterface::login()
{
	system("cls");

	unsigned short int op;
	while (1)
	{
		try 
		{
			cout << "-----------------------------------\n"
				<< "1 -- 登录\n2 -- 返回\n"
				<< "-----------------------------------\n"
				<< "请输入操作码:";
			cin >> op;
			if (cin.fail())throw invalid_argument("请重新输入:");
			if (op == 0 || op > 2) throw invalid_argument("非法操作码!\n请重新输入:");
			if (op == 2)return false;

			string userName;
			string password;
			cout << "-----------------------------------\n"
				<<"请输入用户名和密码\n"
				<< "-----------------------------------\n"
				<<"用户名:";
			cin >> userName;
			cout << "密码:";
			cin >> password;

			if (sys.checkPass(userName, password))
			{
				cout << "登录成功!\n\n";
				ifLogin = true;
				username_t = userName;
				userType = sys.getUserType(username_t);
				return true;
			}
			else 
			{
				cout << "用户名/密码错误.\n";
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

// 展示商品详细信息
void userInterface::displayInfo() const
{
	system("cls");
	sys.displayUserInfo(username_t);
	return;
}

// 重置密码
bool userInterface::setPass()
{
	system("cls");
	string password;
	cout << "请输入新密码:";
	cin >> password;
	if (!cin.fail())
	{
		if (sys.setPass(username_t, password))
			return true;
	}
	return false;
}

// 编辑商品信息
bool userInterface::editItemInfo()
{
	system("cls");
	string name;
	cout << "请输入待修改的商品名称:";
	cin >> name;
	if (sys.ifHaveItem(name, username_t))
	{
		unsigned short int op = 0;
		cout << "-----------------------------------\n"
			<< setw(24) << "1 -- 修改商品描述\n"
			<< setw(24) << "2 -- 修改商品库存\n"
			<< setw(24) << "3 -- 修改商品价格\n"
			<< setw(24) << "4 -- 单品打折促销\n"
			<< setw(24) << "5 -- 品类打折促销\n"
			<< setw(24) << "6 -- 返回上级菜单\n"
			<< "-----------------------------------\n"
			<< "请输入操作码:";
		cin >> op;
		string des;
		double ori = 0, dis = 1.0;
		unsigned int stk = 0;
		int type = sys.getItemType(name, username_t);
		switch (op)
		{
		case SET_DES:
			cout << "\n\n请输入新的商品描述:";
			cin >> des;
			if (sys.setDescript(username_t, name, des))
				return true;
			break;
		case SET_STOCK:
			cout << "\n\n请输入新的商品库存量:";
			cin >> stk;
			if (sys.setStock(username_t, name, stk))
				return true;
			break;
		case SET_PRICE:
			cout << "\n\n请输入新的商品价格:";
			cin >> ori;
			if (sys.setOrigin(username_t, name, ori))
				return true;
			break;
		case SET_DIS:
			cout << "\n\n请输入商品折扣:";
			cin >> dis;
			if (sys.setDiscount(username_t, name, dis))
				return true;
			break;
		case SET_TDIS:
			cout << "\n\n请输入商品折扣:";
			cin >> dis;
			if (sys.setDiscount(username_t, type, dis))
				return true;
			break;
		default:
			return false;
		}
	}
	else
		cout << "商品信息不存在!\n";
	return false;
}

// 注销账号
bool userInterface::delUser()
{
	system("cls");
	char ch;
	cout << "您确定要注销吗? (Y/N)";
	cin >> ch;
	switch (ch)
	{
	case 'Y':
		if (sys.delUser(username_t))
			return true;
		break;
	default:
		break;
	}
	return false;
}

// 充值
bool userInterface::recharge()
{
	system("cls");
	double b = 0;
	cout << "充值金额:";
	cin >> b;
	if (sys.addBalance(username_t, b))
		return true;
	return false;
}

// 添加商品
bool userInterface::addItem()
{
	system("cls");
	string name, descript;
	unsigned int stk;
	int type;
	double ori;
	cout << "请输入商品信息:\n";
	cout << "商品名:";
	cin >> name;
	cout << "\n商品类型( 1 -- 书籍, 2 -- 服装, 3 -- 食品):";
	cin >> type;
	cout << "\n库存:";
	cin >> stk;
	cout << "\n价格:";
	cin >> ori;
	cout << "\n商品描述:";
	cin >> descript;
	if (sys.addItem(name, username_t, descript, ori, stk, type))
		return true;
	return false;
}

// 删除商品
bool userInterface::delItem()
{
	system("cls");
	string name;
	cout << "请输入您要删除的商品名称:";
	cin >> name;
	if (sys.delItem(username_t, name))
		return true;
	return false;
}

// -------------------------------------------------


// 注册界面
bool userInterface::registerUser()
{
	system("cls");

	while (1)
	{
		try
		{
			cout << "-----------------------------------\n"
				<< "1 -- 普通用户注册\n2 -- 商家注册\n3 -- 返回\n"
				<< "请输入操作码:";
			cin >> userType;
			if (cin.fail())throw invalid_argument("请重新输入:");
			if (userType > 3 || userType == 0)throw invalid_argument("非法操作码!\n请重新输入:");
			if (userType == 3)return false;

			string userName;
			string password;
			cout << "请输入用户名和密码\n\n用户名:";
			cin >> userName;
			cout << "密码:";
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

// 查看商品详情
void userInterface::viewDetail(const string& name) const
{
	bool ret = sys.displayItemInfo(name);
	if (!ret)
		cout << "商品信息拉取失败.\n";
	return;
}

// 查看商品
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
			<< "1 -- 查看商品详细信息\n2 -- 返回\n"
			<< "-----------------------------------\n"
			<< "请输入操作码:";
		cin >> op;
		switch (op)
		{
		case 1:
			cout << "-----------------------------------\n"
				<< "请输入查询的商品名称:";
			cin >> name;
			viewDetail(name);
			break;
		default:
			break;
		}
	}
}

// 筛选商品
void userInterface::sortItemList() const
{
	int type = 0;
	system("cls");
	cout << "请选择需要查看的商品类型:\n"
		<< "1 -- 书籍\n2 -- 服装\n3 -- 食品\n\n"
		<< "请输入类型序号:";
	cin >> type;
	if (!cin.fail() && type > 0 && type < 4)
		sys.listItem(type);
	return;
}

// 普通用户界面
void userInterface::crInterface()
{
	unsigned short int op;
	while (1)
	{
		try
		{
			system("cls");
			cout << "-----------------------------------\n"
				<< setw(24) << "3 -- 个人信息\n"
				<< setw(24) << "4 -- 重置密码\n"
				<< setw(24) << "5 -- 余额充值\n"
				<< setw(24) << "6 -- 购物车\n"
				<< setw(24) << "7 -- 订单管理\n"
				<< setw(28) << "9 -- 查看全部商品\n"
				<< setw(30) << "10 -- 按分类筛选商品\n"
				<< setw(24) << "11 -- 退出登录\n"
				<< setw(24) << "12 -- 注销账号\n"
				//<< setw(20) << "14 -- 退出\n"
				<< "-----------------------------------\n"
				<< "请输入操作码:";
			cin >> op;
			if (cin.fail())throw invalid_argument("请重新输入:");
			if (op < 3 || op > 12 || (op < 9 && op > 5))
				throw invalid_argument("非法操作码!\n请重新输入:");
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
				cout << "密码重置成功!\n";
			else
				cout << "重置失败.\n";
			system("pause");
			break;
		case RECHARGE:
			if (recharge())
				cout << "充值成功!\n";
			else
				cout << "充值失败.\n";
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
			system("pause");
			break;
		case DEACTIVE:
			if (delUser())
			{
				cout << "注销成功!\n";
				ifLogin = false;
			}
			else
				cout << "注销失败.\n";
			userType = 0;
			system("pause");
			break;
		}
		if (!ifLogin)show();
		if (userType == 1 && ifLogin)crInterface();
		if (userType == 2 && ifLogin)trInterface();
	}
}

// 商家界面
void userInterface::trInterface()
{
	unsigned short int op;
	while (1)
	{
		try
		{
			system("cls");
			cout << "-----------------------------------\n"
				<< setw(24) << "3 -- 个人信息\n"
				<< setw(24) << "4 -- 重置密码\n"
				<< setw(24) << "5 -- 余额充值\n"
				<< setw(24) << "6 -- 添加商品\n"
				<< setw(28) << "7 -- 修改商品信息\n"
				<< setw(24) << "8 -- 删除商品\n"
				<< setw(28) << "9 -- 查看全部商品\n"
				<< setw(30) << "10 -- 按分类筛选商品\n"
				<< setw(24) << "11 -- 退出登录\n"
				<< setw(24) << "12 -- 注销账号\n"
				//<< setw(20) << "14 -- 退出\n"
				<< "-----------------------------------\n"
				<< "请输入操作码:";
			cin >> op;
			if (cin.fail())throw invalid_argument("请重新输入:");
			if (op < 3 || op > 14 || op == 13)
				throw invalid_argument("非法操作码!\n请重新输入:");
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
				cout << "密码重置成功!\n";
			else
				cout << "重置失败.\n";
			system("pause");
			break;
		case RECHARGE:
			if (recharge())
				cout << "充值成功!\n";
			else
				cout << "充值失败.\n";
			system("pause");
			break;
		case ADD_ITEM:
			if(addItem())
				cout << "添加成功!\n";
			else
				cout << "添加失败.\n";
			system("pause");
			break;
		case EDIT_ITEM_INFO:
			if(editItemInfo())
				cout << "修改成功!\n";
			else
				cout << "修改失败.\n";
			system("pause");
			break;
		case DEL_ITEM:
			if(delItem())
				cout << "删除成功!\n";
			else
				cout << "删除失败.\n";
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
			break;
		case DEACTIVE:
			if (delUser())
			{
				cout << "注销成功!\n";
				ifLogin = false;
			}
			else
				cout << "注销失败.\n";
			userType = 0;
			system("pause");
			break;
		}
		if (!ifLogin)show();
		if (userType == 1 && ifLogin)crInterface();
		if (userType == 2 && ifLogin)trInterface();
	}
}



// 初始界面
void userInterface::show()
{
	unsigned short int op=0;
	while (op!=EXIT)
	{
		try
		{
			system("cls");
			cout << "-----------------------------------\n"
				<< "       欢迎使用xxx电商交易平台       \n"
				<< "-----------------------------------\n"
				<< setw(24) << "1 -- 用户登录\n"
				<< setw(24) << "2 -- 注册账号\n"
				<< setw(24) << "9 -- 查看商品\n"
				<< setw(24) << "14 -- 退出程序\n"
				<< "-----------------------------------\n"
				<< "请输入操作码:";
			cin >> op;
			if (cin.fail())throw invalid_argument("请重新输入:");
			if (op > 2 && op != 9 && op != 14)throw invalid_argument("非法操作码!\n请重新输入:");
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
				std::cerr << "登录失败!\n";
			system("pause");
			break;
		case REG:
			if (registerUser())
				cout << "注册成功!\n\n";
			else
				cout << "注册失败.\n\n";
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
