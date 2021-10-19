#include"ui.h"
#include"user.h"
#include"item.h"

using namespace std;

// 初始界面
void userInterface::show(const string& ip, const string& port)
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

	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(port.c_str()));
	inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

	unsigned short int op = 0;
	while (op != EXIT)
	{
		SOCKET clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		memset(buffRecv, 0, MAX_BUF);
		memset(buffSend, 0, MAX_BUF);

		// 游客模式
		if (!ifLogin)
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
		}

		// 用户界面
		if (ifLogin && userType == CUSTOMER)
		{
			system("cls");
			cout << "-----------------------------------\n"
				<< setw(24) << "3 -- 个人信息\n"
				<< setw(24) << "4 -- 重置密码\n"
				<< setw(24) << "5 -- 余额充值\n"
				<< setw(28) << "9 -- 查看全部商品\n"
				<< setw(30) << "10 -- 按分类筛选商品\n"
				<< setw(24) << "11 -- 退出登录\n"
				<< setw(24) << "12 -- 注销账号\n"
				<< setw(20) << "14 -- 退出\n"
				<< setw(26) << "15 -- 查看购物车\n"
				<< setw(28) << "16 -- 查看订单列表\n"
				<< setw(24) << "22 -- 生成订单\n"
				<< setw(36) << "23 -- 更改购物车内商品数量\n"
				<< setw(32) << "24 -- 删除购物车内商品\n"
				<< setw(26) << "25 -- 清空购物车\n"
				<< setw(24) << "26 -- 删除订单\n"
				<< setw(32) << "27 -- 添加商品至购物车\n"
				<< setw(20) << "28 -- 付款\n"
				<< "-----------------------------------\n"
				<< "请输入操作码:";
			cin >> op;
		}

		// 商家界面
		if (ifLogin && userType == TRADER)
		{
			system("cls");
			cout << "-----------------------------------\n"
				<< setw(24) << "3 -- 个人信息\n"
				<< setw(24) << "4 -- 重置密码\n"
				<< setw(24) << "5 -- 余额充值\n"
				<< setw(24) << "6 -- 添加商品\n"
				<< setw(24) << "8 -- 删除商品\n"
				<< setw(28) << "9 -- 查看全部商品\n"
				<< setw(30) << "10 -- 按分类筛选商品\n"
				<< setw(24) << "11 -- 退出登录\n"
				<< setw(24) << "12 -- 注销账号\n"
				<< setw(20) << "14 -- 退出\n"
				<< setw(28) << "17 -- 更改商品描述\n"
				<< setw(28) << "18 -- 更改商品库存\n"
				<< setw(28) << "19 -- 更改商品价格\n"
				<< setw(28) << "20 -- 更改商品折扣\n"
				<< setw(28) << "21 -- 更改品类折扣\n"
				<< "-----------------------------------\n"
				<< "请输入操作码:";
			cin >> op;
		}

		// 根据操作码执行对应操作
		switch (op)
		{
		// 登录
		case LOGIN:
		{
			if (ifLogin)
			{
				cout << "用户已登录!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "无法连接服务器.\n";
				system("pause");
				return;
			}

			string un, pw;

			cout << "用户名:";
			cin >> un;
			cout << "密码:";
			cin >> pw;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << un << " " << pw;
			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '2')
			{
				cout << "登录成功!\n";
				istrstream iss(buffRecv + 2, MAX_BUF);
				ifLogin = true;
				iss >> token >> userType;
				username_t = un;
			}
			else
			{
				cout << "登录失败.\n";
			}
			system("pause");
			break;
		}

		// 注册
		case REG:
		{
			if (ifLogin)
			{
				cout << "用户已登录!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "无法连接服务器.\n";
				system("pause");
				return;
			}

			string un, pw;
			unsigned int tp;

			cout << "请输入用户信息\n用户名:";
			cin >> un;
			cout << "密码:";
			cin >> pw;
			cout << "账号类型( 1 -- 普通用户, 2 -- 商家 ):";
			cin >> tp;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << un << " " << pw << " " << tp;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "注册成功!\n";
			}
			else
			{
				cout << "注册失败.\n";
			}
			system("pause"); 
			break;
		}

		// 个人信息
		case USER_INFO:
		{
			if (!ifLogin)
			{
				cout << "用户未登录!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "无法连接服务器.\n";
				system("pause");
				return;
			}

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				double bal;
				istrstream iss(buffRecv + 2, MAX_BUF);
				iss >> userType >> bal;
				cout << "用户名:" << username_t
					<< "\n用户类型:" << userType
					<< "\n余额:" << bal;
			}
			else
			{
				cout << "个人信息拉取失败.\n";
			}
			system("pause");
			break;
		}

		// 重置密码
		case RESET_PW:
		{
			if (!ifLogin)
			{
				cout << "用户未登录!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "无法连接服务器.\n";
				system("pause");
				return;
			}

			string pw;

			cout << "请输入新密码:";
			cin >> pw;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token << " " << pw;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "更改成功!\n";
			}
			else
			{
				cout << "更改失败.\n";
			}
			system("pause");
			break;
		}

		// 充值
		case RECHARGE:
		{
			if(!ifLogin)
			{
				cout << "用户未登录!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "无法连接服务器.\n";
				system("pause");
				return;
			}

			double bal;

			cout << "请输入充值金额:";
			cin >> bal;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token << " " << bal;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "充值成功!\n";
			}
			else
			{
				cout << "充值失败.\n";
			}
			system("pause");
			break;
		}

		// 添加商品
		case ADD_ITEM:
		{
			if (!ifLogin)
			{
				cout << "用户未登录!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "无法连接服务器.\n";
				system("pause");
				return;
			}

			string name, des;
			unsigned int tp, stk;
			double ori;

			cout << "请输入商品信息:\n"
				<< "商品名:";
			cin >> name;
			cout << "\n商品类型( 1 -- 书籍, 2 -- 服装, 3 -- 食品):";
			cin >> tp;
			cout << "\n库存:";
			cin >> stk;
			cout << "\n价格:";
			cin >> ori;
			cout << "\n商品描述:";
			cin >> des;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << name << " " << token
				<< " " << des << " " << ori
				<< " " << stk << " " << tp;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "添加成功!\n";
			}
			else
			{
				cout << "添加失败.\n";
			}
			system("pause");
			break;
		}

		// 删除商品
		case DEL_ITEM:
		{
			if (!ifLogin)
			{
				cout << "用户未登录!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "无法连接服务器.\n";
				system("pause");
				return;
			}

			string name;

			cout << "请输入待删除商品名称:";
			cin >> name;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token << " " << name;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "删除成功!\n";
			}
			else
			{
				cout << "删除失败.\n";
			}
			system("pause");
			break;
		}

		// 查看全部商品
		case LIST_ALL_ITEM:
		{
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "无法连接服务器.\n";
				system("pause");
				return;
			}

			ostrstream oss(buffSend, MAX_BUF);
			oss << op;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				istringstream iss(buffRecv+2, MAX_BUF);
				string name;
				unsigned int stk, type;
				double pri;
				
				cout << setw(20) << "商品名称"
					<< setw(20) << "价格"
					<< setw(20) << "类型"
					<< setw(20) << "库存"
					<< endl;
				while (iss >> name >> pri >> type >> stk)
				{
					cout << setw(20) << name
						<< setw(20) << pri
						<< setw(20) << type
						<< setw(20) << stk
						<< endl;
				}
			}
			else
			{
				cout << "信息获取失败.\n";
			}
			system("pause");
			break;
		}

		// 按分类筛选商品
		case SORT_ITEM:
		{
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "无法连接服务器.\n";
				system("pause");
				return;
			}

			unsigned int tp;
			cout << "请输入筛选商品类型:";
			cin >> tp;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op<<" " <<tp;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				istringstream iss(buffRecv+2, MAX_BUF);
				string name, owner;
				unsigned int stk, type;
				double pri;
				cout << setw(20) << "商品名称"
					<< setw(20) << "价格"
					<< setw(20) << "类型"
					<< setw(20) << "库存"
					<< endl;
				while(iss >> name >> pri >> type >> stk)
				{
					cout << setw(20) << name
						<< setw(20) << pri
						<< setw(20) << type
						<< setw(20) << stk
						<< endl;
				}
			}
			else
			{
				cout << "信息获取失败.\n";
			}
			system("pause");
			break;
		}

		// 登出
		case LOGOUT:
		{
			if (!ifLogin)
			{
				cout << "用户未登录!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "无法连接服务器.\n";
				system("pause");
				return;
			}

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				ifLogin = false;
				cout << "成功登出.\n";
			}
			else
			{
				cout << "登出失败.\n";
			}
			system("pause");
			break;
		}

		// 注销账号
		case DEACTIVE :
		{
			if(!ifLogin)
			{
				cout << "用户未登录!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "无法连接服务器.\n";
				system("pause");
				return;
			}

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				ifLogin = false;
				cout << "注销成功!\n";
			}
			else
			{
				cout << "注销失败.\n";
			}
			system("pause");
			break;
		}

		// 退出
		case EXIT:
		{
			break;
		}

		// 查看购物车
		case 15:
		{
			if (!ifLogin)
				if (!ifLogin)
				{
					cout << "用户未登录!\n";
					break;
				}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "无法连接服务器.\n";
				system("pause");
				return;
			}

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				istringstream iss(buffRecv+2, MAX_BUF);
				string name;
				unsigned int n;
				cout << setw(20) << "商品名称"
					<< setw(20) << "数量"
					<< endl;
				while (iss >> name >> n)
				{
					cout << setw(20) << name
						<< setw(20) << n
						<< endl;
				}
			}
			else
			{
				cout << "信息拉取失败.\n";
			}
			system("pause");
			break;
		}

		// 查看订单列表
		case 16:
		{
			if (!ifLogin)
			{
				cout << "用户未登录!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "无法连接服务器.\n";
				system("pause");
				return;
			}

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				istringstream iss(buffRecv+2, MAX_BUF);
				string name;
				unsigned int n;
				double sum;
				cout << setw(20) << "商品名称"
					<< setw(20) << "数量"
					<< endl;
				iss >> sum;
				while (iss >> name >> n)
				{
					cout << setw(20) << name
						<< setw(20) << n
						<< endl;
				}
				cout << "总金额:" << sum << endl;
			}
			else
			{
				cout << "信息拉取失败.\n";
			}
			system("pause");
			break;
		}

		// 编辑商品描述
		case 17:
		{
			if (!ifLogin)
			{
				cout << "用户未登录!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "无法连接服务器.\n";
				system("pause");
				return;
			}

			string name, des;

			cout << "请输入商品名称及描述:";
			cin >> name >> des;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token << " " << name << " " << des;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "修改成功!\n";
			}
			else
			{
				cout << "修改失败.\n";
			}
			system("pause");
			break;
		}

		// 编辑商品库存
		case 18:
		{
			if (!ifLogin)
			{
				cout << "用户未登录!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "无法连接服务器.\n";
				system("pause");
				return;
			}

			string name;
			unsigned int stk;

			cout << "请输入商品名称及库存:";
			cin >> name >> stk;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token << " " << name << " " << stk;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "修改成功!\n";
			}
			else
			{
				cout << "修改失败.\n";
			}
			system("pause");
			break;
		}

		// 编辑商品价格
		case 19:
		{
			if (!ifLogin)
			{
				cout << "用户未登录!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "无法连接服务器.\n";
				system("pause");
				return;
			}

			string name;
			double ori;

			cout << "请输入商品名称及价格:";
			cin >> name >> ori;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token << " " << name << " " << ori;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "修改成功!\n";
			}
			else
			{
				cout << "修改失败.\n";
			}
			system("pause");
			break;
		}

		// 编辑单品折扣
		case 20:
		{
			if (!ifLogin)
			{
				cout << "用户未登录!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "无法连接服务器.\n";
				system("pause");
				return;
			}

			string name;
			double dis;

			cout << "请输入商品名称及折扣:";
			cin >> name >> dis;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token << " " << name << " " << dis;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "修改成功!\n";
			}
			else
			{
				cout << "修改失败.\n";
			}
			system("pause");
			break;
		}

		// 品类打折
		case 21:
		{
			if (!ifLogin)
			{
				cout << "用户未登录!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "无法连接服务器.\n";
				system("pause");
				return;
			}

			unsigned int tp;
			double dis;

			cout << "请输入商品类型及折扣:";
			cin >> tp >> dis;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token << " " << tp << " " << dis;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "修改成功!\n";
			}
			else
			{
				cout << "修改失败.\n";
			}
			system("pause");
			break;
		}

		// 生成订单
		case 22:
		{
			if (!ifLogin)
			{
				cout << "用户未登录!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "无法连接服务器.\n";
				system("pause");
				return;
			}

			string name;

			cout << "-----------------------------------\n"
				<< "请输入购买的商品名:";
			cin >> name;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token << " " << name;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "订单生成成功!\n";
			}
			else
			{
				cout << "生成失败.\n";
			}
			system("pause");
			break;
		}

		// 编辑购物车商品数量
		case 23:
		{
			if (!ifLogin)
			{
				cout << "用户未登录!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "无法连接服务器.\n";
				system("pause");
				return;
			}

			string name;
			unsigned int num;

			cout << "-----------------------------------\n"
				<< "请输入要修改的商品名和数量:";
			cin >> name >> num;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token << " " << name << " " << num;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "修改成功!\n";
			}
			else
			{
				cout << "修改失败.\n";
			}
			system("pause");
			break;
		}

		// 删除购物车内商品
		case 24:
		{
			if (!ifLogin)
			{
				cout << "用户未登录!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "无法连接服务器.\n";
				system("pause");
				return;
			}

			string name;

			cout << "\n请输入待删除的商品名:";
			cin >> name;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token << " " << name;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "删除成功!\n";
			}
			else
			{
				cout << "删除失败.\n";
			}
			system("pause");
			break;
		}

		// 清空购物车
		case 25:
		{
			if (!ifLogin)
			{
				cout << "用户未登录!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "无法连接服务器.\n";
				system("pause");
				return;
			}

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "清空成功!\n";
			}
			else
			{
				cout << "清空失败.\n";
			}
			system("pause");
			break;
		}

		// 删除订单
		case 26:
		{
			if (!ifLogin)
			{
				cout << "用户未登录!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "无法连接服务器.\n";
				system("pause");
				return;
			}

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "删除成功!\n";
			}
			else
			{
				cout << "删除失败.\n";
			}
			system("pause");
			break;
		}

		// 添加商品至购物车
		case 27:
		{
			if (!ifLogin)
			{
				cout << "用户未登录!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "无法连接服务器.\n";
				system("pause");
				return;
			}
			
			string name;
			unsigned int num;

			cout << "-----------------------------------\n"
				<< "请输入要添加的商品名和数量:";
			cin >> name >> num;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token << " " << name << " " << num;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "添加成功!\n";
			}
			else
			{
				cout << "添加失败.\n";
			}
			system("pause");
			break;
		}

		// 支付
		case 28:
		{
			if (!ifLogin)
			{
				cout << "用户未登录!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "无法连接服务器.\n";
				system("pause");
				return;
			}

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "支付成功!\n";
			}
			else
			{
				cout << "支付失败.\n";
			}
			system("pause");
			break;
		}

		default:
		{
			cout << "无效操作码.\n请重新输入:";
			cin >> op;
			system("pause");
			break;
		}
		}
		closesocket(clientSock);
		
	}
	exit(0);
}
