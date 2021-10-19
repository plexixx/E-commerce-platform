#include"user.h"

// ---------------- User ----------------

// ע����
bool User::ifValidUserPass(const string& _name, const string& _password)
{
	// �������Ϸ���
	if (_name.length() < 5 || _name.length() > 10)
	{
		cout << "Invalid name!\nPlease enter a new name.(5-10)\n\n";
		return false;
	}
	if (_password.length() < 6 || _password.length() > 15)
	{
		cout << "Invalid Password!\nPlease reenter the security password.(6-15)\n\n";
		return false;
	}
	if (_password.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos
		|| _name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		std::cerr << "Error!\nname/Password can only contain a-z, A-Z, 0-9 and _\n\n";
		return false;
	}

	name = _name;
	password = _password;
	return true;
}

// �޸�����
bool User::setPass(const string& newpw)
{
	// �������Ϸ���
	if (newpw.length() < 6 || newpw.length() > 15)
	{
		cout << "Invalid Password!\nPlease reenter the new security password.(6-15)\n\n";
		return false;
	}
	if (newpw.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
	{
		std::cerr << "Error!\nPassword can only contain a-z, A-Z, 0-9 and _\n\n";
		return false;
	}
	password = newpw;
	return true;
}

// �������
bool User::addBalance(double b)
{
	if (b > 0) 
	{
		balance += b;
		return true;
	}
	return false;
}

// ������
bool User::subBalance(double b)
{
	if (b > 0 && balance >= b)
	{
		balance -= b;
		return true;
	}
	return false;
}


// ---------------- Trader ----------------

// ---------------- Customer ----------------

// �����Ʒ�����ﳵ
bool Customer::addToCart(const string& name, unsigned int num)
{
	for (auto it = cart.begin(); it != cart.end(); it++)
	{
		if (name.compare(it->first) == 0) 
		{
			it->second += num;
			return true;
		}
	}
	auto ret = cart.insert(pair<string, int>(name, num));
	return ret.second;
}

// �Ƴ����ﳵ��ָ����Ʒ
bool Customer::removeFromCart(const string& name)
{
	if (cart.size() == 0)return false;
	for (auto it = cart.begin(); it != cart.end(); it++)
	{
		if (name.compare(it->first) == 0)
		{
			cart.erase(name);
			return true;
		}
	}
	return false;
}

// �Ƴ����ﳵ��һ��������ָ����Ʒ
bool Customer::removeFromCart(const string& name, unsigned int num)
{
	if (cart.size() == 0)return false;
	for (auto it = cart.begin(); it != cart.end(); it++)
	{
		if (name.compare(it->first) == 0)
		{
			if (num > it->second)return false;
			else if (num < it->second) it->second -= num;
			else
				cart.erase(name);
			return true;
		}
	}
	return false;
}

// �޸Ĺ��ﳵ����Ʒ����
bool Customer::setItemNum(const string& name, unsigned int num)
{
	if (cart.size() == 0)return false;
	for (auto it = cart.begin(); it != cart.end(); it++)
	{
		if (name.compare(it->first) == 0)
		{
			if (num == 0)
				cart.erase(name);
			else
				it->second = num;
			return true;
		}
	}
	return false;
}

// ���ɶ���
bool Customer::generateOrder(const string& name, unsigned int stock)
{
	if (cart.size() == 0)return false;
	for (auto it = cart.begin(); it != cart.end(); it++)
	{
		if (name.compare(it->first) == 0)
		{
			string str = it->first;
			unsigned int num = it->second;

			// �Ϸ��Լ�飬ȷ�����������������ڿ��
			if (num <= stock)
			{
				cart.erase(str);
				auto ret = order.insert(pair<string, int>(str, num));
				return ret.second;
			}
		}
	}
	return false;
}