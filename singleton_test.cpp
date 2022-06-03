//
// Created by yuval on 6/3/22.
//

#include "singleton.hpp"

using namespace std;
using namespace ex6;

int main(){ // lock a file using singleton
    FILE *file = fopen("test_lock.txt","w+a+");
    auto* singleton_ok = Singleton<FILE*>::Instance(file);
    auto* singleton_err = Singleton<FILE*>::Instance(file);
    cout << "singleton_ok: " << singleton_ok << endl;
    cout << "singleton_err: " << singleton_err << endl;
    if(singleton_ok == singleton_err){
        cout << "singleton_ok == singleton_err that means we are good!" << endl;
    } else {
        cout << "singleton_ok != singleton_err that means we are bad!" << endl;
    }
    fclose(file);
    return 0;
}