#include"ui.h"
#include"user.h"
#include"item.h"

using namespace std;

// ��ʼ����
void userInterface::show(const string& ip, const string& port)
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

		// �ο�ģʽ
		if (!ifLogin)
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
		}

		// �û�����
		if (ifLogin && userType == CUSTOMER)
		{
			system("cls");
			cout << "-----------------------------------\n"
				<< setw(24) << "3 -- ������Ϣ\n"
				<< setw(24) << "4 -- ��������\n"
				<< setw(24) << "5 -- ����ֵ\n"
				<< setw(28) << "9 -- �鿴ȫ����Ʒ\n"
				<< setw(30) << "10 -- ������ɸѡ��Ʒ\n"
				<< setw(24) << "11 -- �˳���¼\n"
				<< setw(24) << "12 -- ע���˺�\n"
				<< setw(20) << "14 -- �˳�\n"
				<< setw(26) << "15 -- �鿴���ﳵ\n"
				<< setw(28) << "16 -- �鿴�����б�\n"
				<< setw(24) << "22 -- ���ɶ���\n"
				<< setw(36) << "23 -- ���Ĺ��ﳵ����Ʒ����\n"
				<< setw(32) << "24 -- ɾ�����ﳵ����Ʒ\n"
				<< setw(26) << "25 -- ��չ��ﳵ\n"
				<< setw(24) << "26 -- ɾ������\n"
				<< setw(32) << "27 -- �����Ʒ�����ﳵ\n"
				<< setw(20) << "28 -- ����\n"
				<< "-----------------------------------\n"
				<< "�����������:";
			cin >> op;
		}

		// �̼ҽ���
		if (ifLogin && userType == TRADER)
		{
			system("cls");
			cout << "-----------------------------------\n"
				<< setw(24) << "3 -- ������Ϣ\n"
				<< setw(24) << "4 -- ��������\n"
				<< setw(24) << "5 -- ����ֵ\n"
				<< setw(24) << "6 -- �����Ʒ\n"
				<< setw(24) << "8 -- ɾ����Ʒ\n"
				<< setw(28) << "9 -- �鿴ȫ����Ʒ\n"
				<< setw(30) << "10 -- ������ɸѡ��Ʒ\n"
				<< setw(24) << "11 -- �˳���¼\n"
				<< setw(24) << "12 -- ע���˺�\n"
				<< setw(20) << "14 -- �˳�\n"
				<< setw(28) << "17 -- ������Ʒ����\n"
				<< setw(28) << "18 -- ������Ʒ���\n"
				<< setw(28) << "19 -- ������Ʒ�۸�\n"
				<< setw(28) << "20 -- ������Ʒ�ۿ�\n"
				<< setw(28) << "21 -- ����Ʒ���ۿ�\n"
				<< "-----------------------------------\n"
				<< "�����������:";
			cin >> op;
		}

		// ���ݲ�����ִ�ж�Ӧ����
		switch (op)
		{
		// ��¼
		case LOGIN:
		{
			if (ifLogin)
			{
				cout << "�û��ѵ�¼!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "�޷����ӷ�����.\n";
				system("pause");
				return;
			}

			string un, pw;

			cout << "�û���:";
			cin >> un;
			cout << "����:";
			cin >> pw;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << un << " " << pw;
			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '2')
			{
				cout << "��¼�ɹ�!\n";
				istrstream iss(buffRecv + 2, MAX_BUF);
				ifLogin = true;
				iss >> token >> userType;
				username_t = un;
			}
			else
			{
				cout << "��¼ʧ��.\n";
			}
			system("pause");
			break;
		}

		// ע��
		case REG:
		{
			if (ifLogin)
			{
				cout << "�û��ѵ�¼!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "�޷����ӷ�����.\n";
				system("pause");
				return;
			}

			string un, pw;
			unsigned int tp;

			cout << "�������û���Ϣ\n�û���:";
			cin >> un;
			cout << "����:";
			cin >> pw;
			cout << "�˺�����( 1 -- ��ͨ�û�, 2 -- �̼� ):";
			cin >> tp;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << un << " " << pw << " " << tp;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "ע��ɹ�!\n";
			}
			else
			{
				cout << "ע��ʧ��.\n";
			}
			system("pause"); 
			break;
		}

		// ������Ϣ
		case USER_INFO:
		{
			if (!ifLogin)
			{
				cout << "�û�δ��¼!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "�޷����ӷ�����.\n";
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
				cout << "�û���:" << username_t
					<< "\n�û�����:" << userType
					<< "\n���:" << bal;
			}
			else
			{
				cout << "������Ϣ��ȡʧ��.\n";
			}
			system("pause");
			break;
		}

		// ��������
		case RESET_PW:
		{
			if (!ifLogin)
			{
				cout << "�û�δ��¼!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "�޷����ӷ�����.\n";
				system("pause");
				return;
			}

			string pw;

			cout << "������������:";
			cin >> pw;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token << " " << pw;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "���ĳɹ�!\n";
			}
			else
			{
				cout << "����ʧ��.\n";
			}
			system("pause");
			break;
		}

		// ��ֵ
		case RECHARGE:
		{
			if(!ifLogin)
			{
				cout << "�û�δ��¼!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "�޷����ӷ�����.\n";
				system("pause");
				return;
			}

			double bal;

			cout << "�������ֵ���:";
			cin >> bal;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token << " " << bal;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "��ֵ�ɹ�!\n";
			}
			else
			{
				cout << "��ֵʧ��.\n";
			}
			system("pause");
			break;
		}

		// �����Ʒ
		case ADD_ITEM:
		{
			if (!ifLogin)
			{
				cout << "�û�δ��¼!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "�޷����ӷ�����.\n";
				system("pause");
				return;
			}

			string name, des;
			unsigned int tp, stk;
			double ori;

			cout << "��������Ʒ��Ϣ:\n"
				<< "��Ʒ��:";
			cin >> name;
			cout << "\n��Ʒ����( 1 -- �鼮, 2 -- ��װ, 3 -- ʳƷ):";
			cin >> tp;
			cout << "\n���:";
			cin >> stk;
			cout << "\n�۸�:";
			cin >> ori;
			cout << "\n��Ʒ����:";
			cin >> des;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << name << " " << token
				<< " " << des << " " << ori
				<< " " << stk << " " << tp;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "��ӳɹ�!\n";
			}
			else
			{
				cout << "���ʧ��.\n";
			}
			system("pause");
			break;
		}

		// ɾ����Ʒ
		case DEL_ITEM:
		{
			if (!ifLogin)
			{
				cout << "�û�δ��¼!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "�޷����ӷ�����.\n";
				system("pause");
				return;
			}

			string name;

			cout << "�������ɾ����Ʒ����:";
			cin >> name;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token << " " << name;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "ɾ���ɹ�!\n";
			}
			else
			{
				cout << "ɾ��ʧ��.\n";
			}
			system("pause");
			break;
		}

		// �鿴ȫ����Ʒ
		case LIST_ALL_ITEM:
		{
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "�޷����ӷ�����.\n";
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
				
				cout << setw(20) << "��Ʒ����"
					<< setw(20) << "�۸�"
					<< setw(20) << "����"
					<< setw(20) << "���"
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
				cout << "��Ϣ��ȡʧ��.\n";
			}
			system("pause");
			break;
		}

		// ������ɸѡ��Ʒ
		case SORT_ITEM:
		{
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "�޷����ӷ�����.\n";
				system("pause");
				return;
			}

			unsigned int tp;
			cout << "������ɸѡ��Ʒ����:";
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
				cout << setw(20) << "��Ʒ����"
					<< setw(20) << "�۸�"
					<< setw(20) << "����"
					<< setw(20) << "���"
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
				cout << "��Ϣ��ȡʧ��.\n";
			}
			system("pause");
			break;
		}

		// �ǳ�
		case LOGOUT:
		{
			if (!ifLogin)
			{
				cout << "�û�δ��¼!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "�޷����ӷ�����.\n";
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
				cout << "�ɹ��ǳ�.\n";
			}
			else
			{
				cout << "�ǳ�ʧ��.\n";
			}
			system("pause");
			break;
		}

		// ע���˺�
		case DEACTIVE :
		{
			if(!ifLogin)
			{
				cout << "�û�δ��¼!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "�޷����ӷ�����.\n";
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
				cout << "ע���ɹ�!\n";
			}
			else
			{
				cout << "ע��ʧ��.\n";
			}
			system("pause");
			break;
		}

		// �˳�
		case EXIT:
		{
			break;
		}

		// �鿴���ﳵ
		case 15:
		{
			if (!ifLogin)
				if (!ifLogin)
				{
					cout << "�û�δ��¼!\n";
					break;
				}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "�޷����ӷ�����.\n";
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
				cout << setw(20) << "��Ʒ����"
					<< setw(20) << "����"
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
				cout << "��Ϣ��ȡʧ��.\n";
			}
			system("pause");
			break;
		}

		// �鿴�����б�
		case 16:
		{
			if (!ifLogin)
			{
				cout << "�û�δ��¼!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "�޷����ӷ�����.\n";
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
				cout << setw(20) << "��Ʒ����"
					<< setw(20) << "����"
					<< endl;
				iss >> sum;
				while (iss >> name >> n)
				{
					cout << setw(20) << name
						<< setw(20) << n
						<< endl;
				}
				cout << "�ܽ��:" << sum << endl;
			}
			else
			{
				cout << "��Ϣ��ȡʧ��.\n";
			}
			system("pause");
			break;
		}

		// �༭��Ʒ����
		case 17:
		{
			if (!ifLogin)
			{
				cout << "�û�δ��¼!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "�޷����ӷ�����.\n";
				system("pause");
				return;
			}

			string name, des;

			cout << "��������Ʒ���Ƽ�����:";
			cin >> name >> des;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token << " " << name << " " << des;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "�޸ĳɹ�!\n";
			}
			else
			{
				cout << "�޸�ʧ��.\n";
			}
			system("pause");
			break;
		}

		// �༭��Ʒ���
		case 18:
		{
			if (!ifLogin)
			{
				cout << "�û�δ��¼!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "�޷����ӷ�����.\n";
				system("pause");
				return;
			}

			string name;
			unsigned int stk;

			cout << "��������Ʒ���Ƽ����:";
			cin >> name >> stk;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token << " " << name << " " << stk;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "�޸ĳɹ�!\n";
			}
			else
			{
				cout << "�޸�ʧ��.\n";
			}
			system("pause");
			break;
		}

		// �༭��Ʒ�۸�
		case 19:
		{
			if (!ifLogin)
			{
				cout << "�û�δ��¼!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "�޷����ӷ�����.\n";
				system("pause");
				return;
			}

			string name;
			double ori;

			cout << "��������Ʒ���Ƽ��۸�:";
			cin >> name >> ori;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token << " " << name << " " << ori;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "�޸ĳɹ�!\n";
			}
			else
			{
				cout << "�޸�ʧ��.\n";
			}
			system("pause");
			break;
		}

		// �༭��Ʒ�ۿ�
		case 20:
		{
			if (!ifLogin)
			{
				cout << "�û�δ��¼!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "�޷����ӷ�����.\n";
				system("pause");
				return;
			}

			string name;
			double dis;

			cout << "��������Ʒ���Ƽ��ۿ�:";
			cin >> name >> dis;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token << " " << name << " " << dis;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "�޸ĳɹ�!\n";
			}
			else
			{
				cout << "�޸�ʧ��.\n";
			}
			system("pause");
			break;
		}

		// Ʒ�����
		case 21:
		{
			if (!ifLogin)
			{
				cout << "�û�δ��¼!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "�޷����ӷ�����.\n";
				system("pause");
				return;
			}

			unsigned int tp;
			double dis;

			cout << "��������Ʒ���ͼ��ۿ�:";
			cin >> tp >> dis;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token << " " << tp << " " << dis;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "�޸ĳɹ�!\n";
			}
			else
			{
				cout << "�޸�ʧ��.\n";
			}
			system("pause");
			break;
		}

		// ���ɶ���
		case 22:
		{
			if (!ifLogin)
			{
				cout << "�û�δ��¼!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "�޷����ӷ�����.\n";
				system("pause");
				return;
			}

			string name;

			cout << "-----------------------------------\n"
				<< "�����빺�����Ʒ��:";
			cin >> name;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token << " " << name;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "�������ɳɹ�!\n";
			}
			else
			{
				cout << "����ʧ��.\n";
			}
			system("pause");
			break;
		}

		// �༭���ﳵ��Ʒ����
		case 23:
		{
			if (!ifLogin)
			{
				cout << "�û�δ��¼!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "�޷����ӷ�����.\n";
				system("pause");
				return;
			}

			string name;
			unsigned int num;

			cout << "-----------------------------------\n"
				<< "������Ҫ�޸ĵ���Ʒ��������:";
			cin >> name >> num;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token << " " << name << " " << num;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "�޸ĳɹ�!\n";
			}
			else
			{
				cout << "�޸�ʧ��.\n";
			}
			system("pause");
			break;
		}

		// ɾ�����ﳵ����Ʒ
		case 24:
		{
			if (!ifLogin)
			{
				cout << "�û�δ��¼!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "�޷����ӷ�����.\n";
				system("pause");
				return;
			}

			string name;

			cout << "\n�������ɾ������Ʒ��:";
			cin >> name;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token << " " << name;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "ɾ���ɹ�!\n";
			}
			else
			{
				cout << "ɾ��ʧ��.\n";
			}
			system("pause");
			break;
		}

		// ��չ��ﳵ
		case 25:
		{
			if (!ifLogin)
			{
				cout << "�û�δ��¼!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "�޷����ӷ�����.\n";
				system("pause");
				return;
			}

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "��ճɹ�!\n";
			}
			else
			{
				cout << "���ʧ��.\n";
			}
			system("pause");
			break;
		}

		// ɾ������
		case 26:
		{
			if (!ifLogin)
			{
				cout << "�û�δ��¼!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "�޷����ӷ�����.\n";
				system("pause");
				return;
			}

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "ɾ���ɹ�!\n";
			}
			else
			{
				cout << "ɾ��ʧ��.\n";
			}
			system("pause");
			break;
		}

		// �����Ʒ�����ﳵ
		case 27:
		{
			if (!ifLogin)
			{
				cout << "�û�δ��¼!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "�޷����ӷ�����.\n";
				system("pause");
				return;
			}
			
			string name;
			unsigned int num;

			cout << "-----------------------------------\n"
				<< "������Ҫ��ӵ���Ʒ��������:";
			cin >> name >> num;

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token << " " << name << " " << num;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "��ӳɹ�!\n";
			}
			else
			{
				cout << "���ʧ��.\n";
			}
			system("pause");
			break;
		}

		// ֧��
		case 28:
		{
			if (!ifLogin)
			{
				cout << "�û�δ��¼!\n";
				break;
			}
			if (connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
			{
				cout << "�޷����ӷ�����.\n";
				system("pause");
				return;
			}

			ostrstream oss(buffSend, MAX_BUF);
			oss << op << " " << token;

			send(clientSock, buffSend, MAX_BUF, 0);
			recv(clientSock, buffRecv, MAX_BUF, 0);

			if (buffRecv[0] == '1')
			{
				cout << "֧���ɹ�!\n";
			}
			else
			{
				cout << "֧��ʧ��.\n";
			}
			system("pause");
			break;
		}

		default:
		{
			cout << "��Ч������.\n����������:";
			cin >> op;
			system("pause");
			break;
		}
		}
		closesocket(clientSock);
		
	}
	exit(0);
}
