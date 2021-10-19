#include"item.h"

// ---------------- Item ----------------

// ������Ʒ����
bool Item::setItemName(const string& newName)
{
	// �������Ϸ���
	if (newName.length() < 4 || newName.length() > 15)
	{
		cout << "Invalid Item Name!\nPlease reenter the new name.(4-15)\n\n";
		return false;
	}
	if (newName.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		std::cerr << "Error!\nItem Name can only contain a-z, A-Z, 0-9 and _\n\n";
		return false;
	}

	name = newName;
	return true;
}

// ������Ʒ����
bool Item::setDescript(const string& newdes)
{
	// �������Ϸ���
	if (newdes.length() < 10 || newdes.length() > 100)
	{
		cout << "Invalid Item Description!\nPlease reenter.(10-100)\n\n";
		return false;
	}
	if (newdes.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		std::cerr << "Error!\nItem Description can only contain a-z, A-Z, 0-9 and _\n\n";
		return false;
	}

	descript = newdes;
	return true;
}

// ������Ʒ�̼���Ϣ
bool Item::setOwner(const string& username)
{
	if (username.empty())return false;

	owner = username;
	return true;
}

// ������Ʒ�ۿ�
bool Item::setDiscount(const double& newdis)
{
	if (newdis < 0 || newdis>1)
	{
		std::cerr << "Error!\nDiscount must be in (0, 1].\n\n";
		return false;
	}

	discount = newdis;
	return true;
}

// ������Ʒ���
bool Item::setStock(const unsigned int& newstk)
{
	if (newstk < MIN_STOCK || newstk>MAX_STOCK)
	{
		std::cerr << "Error!\nStock must be in [0, 9999].\n\n";
		return false;
	}

	stock = newstk;
	return true;
}

// ������Ʒԭ��
bool Item::setOrigin(double ori)
{
	if (ori < 0)
	{
		std::cerr << "Error!\nOriginal Price must be positive.\n\n";
		return false;
	}
	origin = ori;
	return true;
}

// չʾ��Ʒ��Ϣ
void Item::displayItemInfo()
{
	system("cls");
	cout << "-----------------------------------" << endl
		<< setw(10) << "��Ʒ����:" << name << endl
		<< setw(10) << "��Ʒ����:" << descript << endl
		<< setw(10) << "���:" << stock << endl
		<< setw(10) << "ԭ��:" << origin << endl
		<< setw(10) << "�ּ�:" << getPrice() << endl
		<< setw(10) << "�̼�����:" << owner << endl
		<< "-----------------------------------" <<endl;
}

// ��ʼ��Ʒ�������Ϣ
double Book::dis = 1.0;
double Cloth::dis = 1.0;
double Food::dis = 1.0;

// ---------------- Book ----------------

// ����Ʒ���ۿ�
bool Book::setDiscount(double d) 
{
	if (d < 0 || d > 1)
	{
		std::cerr << "Error!\nDiscount must be in (0, 1].\n\n";
		return false;
	}
	Book::dis = d;
	return true;
}

// ---------------- Cloth ----------------

bool Cloth::setDiscount(double d)
{
	if (d < 0 || d > 1)
	{
		std::cerr << "Error!\nDiscount must be in (0, 1].\n\n";
		return false;
	}
	Cloth::dis = d;
	return true;
}


// ---------------- Food ----------------

bool Food::setDiscount(double d)
{
	if (d < 0 || d > 1)
	{
		std::cerr << "Error!\nDiscount must be in (0, 1].\n\n";
		return false;
	}
	Food::dis = d;
	return true;
}
