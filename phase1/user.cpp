#include"user.h"

// ---------------- User ----------------

// 注册检查
bool User::ifValidUserPass(const string& _name, const string& _password)
{
	// 检查输入合法性
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

// 修改密码
bool User::setPass(const string& newpw)
{
	// 检查输入合法性
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

// 余额修改
bool User::addBalance(double b)
{
	if (b > 0) 
	{
		balance += b;
		return true;
	}
	return false;
}