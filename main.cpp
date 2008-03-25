#include <iostream>
#include "socktroll.hpp"

using namespace std;

int main( int argc, int *argv )
{
    Network* net = new Network( "127.0.0.1", 6000 );
    cout << "socktroll client written in C++" << endl;

    net->send( "nick kurt" );
    
    delete net;
    return 0;
}
