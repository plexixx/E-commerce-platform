#include"global.h"

bool processError(std::invalid_argument err)
{
    using namespace std;

    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cout << err.what() << "\nTry again? Enter y or n" << endl;
    char c;
    cin >> c;
    if (!cin || c == 'n') return false;
    else return true;
}