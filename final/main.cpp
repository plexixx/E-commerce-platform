#include"ui.h"

using namespace std;

int main(int argc, char* argv[])
{
	if (argc <= 1)
	{
		cout << "ʹ��˵��:\n"
			<< "����������: app -s port\n"
			<< "�����ͻ���: app -c ip port\n";
	}
	else
	{
		if (strcmp(argv[1], "-s") == 0)
		{
			if (argc < 3)
				cout << "������˿ں�.\n";
			else
			{
				ESystem& sys = ESystem::getInstance();
				sys.readItemFile();
				sys.readUserFile();
				sys.run(argv[2]);
			}
		}
		else if (strcmp(argv[1], "-c") == 0)
		{
			if (argc < 4)
				cout << "������ip��ַ�Ͷ˿ں�.\n";
			else
			{
				userInterface mainwindow;
				mainwindow.show(argv[2], argv[3]);
			}
		}
		else
		{
			cout << "ʹ��˵��:\n"
				<< "����������: -s port\n"
				<< "�����ͻ���: -c ip port\n";
		}
	}
	
	return 0;
}