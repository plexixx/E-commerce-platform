#include"ui.h"

using namespace std;

int main(int argc, char* argv[])
{
	if (argc <= 1)
	{
		cout << "使用说明:\n"
			<< "启动服务器: app -s port\n"
			<< "启动客户端: app -c ip port\n";
	}
	else
	{
		if (strcmp(argv[1], "-s") == 0)
		{
			if (argc < 3)
				cout << "请输入端口号.\n";
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
				cout << "请输入ip地址和端口号.\n";
			else
			{
				userInterface mainwindow;
				mainwindow.show(argv[2], argv[3]);
			}
		}
		else
		{
			cout << "使用说明:\n"
				<< "启动服务器: -s port\n"
				<< "启动客户端: -c ip port\n";
		}
	}
	
	return 0;
}