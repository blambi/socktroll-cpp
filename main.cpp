#include <iostream>
#include "socktroll.hpp"

using namespace std;

int main( int argc, int *argv )
{
    Network* net = new Network( "127.0.0.1", 6000 );
    string temp;

    cout << "socktroll client written in C++" << endl;

    cout << net->recv();

    net->send( "nick kurt" );
    temp = net->recv();

    cout << temp << endl;
    cout << temp.size() << endl;
    
    net->send( "msg kaka" );
    
    delete net;
    return 0;
}
