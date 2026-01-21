#include "User.h"

User::User(string u, string p, bool admin)
    : username(u), password(p), isAdmin(admin) {
}

ofstream& operator<<(ofstream& fout, const User& user) {
    fout << user.username << " ";
    fout << user.password << " ";
    fout << (user.isAdmin ? 1 : 0);
    return fout;
}

ifstream& operator>>(ifstream& fin, User& user) {
    int adminFlag;
    fin >> user.username;
    fin >> user.password;
    fin >> adminFlag;
    user.isAdmin = (adminFlag == 1);
    return fin;
}