#include <string>
#include "socktroll.hpp"

Protocol::Protocol( void )
{
    nick = std::string( "" );
    authed = false;
}

Protocol::~Protocol( void )
{
    if( net and authed )
        net->send( "quit" );
}

void Protocol::parse( std::string message )
{
    int del;
    std::string temp;

    /* FIXME: not that pretty but it works */
    if( message.substr( 0, 3 ) == "msg" )
    {  /* normal message */
        temp = message.substr( 4 );
        temp = temp.substr( 0, temp.find( " " ));
            ui->print( "<%s> %s", temp.c_str(),
                       message.substr( 5 + temp.size() ).c_str() );
    }
    else if( message.substr( 0, 1 ) == "+" or message.substr( 0, 1 ) == "-" )
    { /* join / part */
        ui->print( " %s %s", message.substr( 0, 1 ).c_str(),
                   message.substr( 2 ).c_str() );
    }
    else if( message.substr( 0, 6 ) == "action" )
    {  /* "/me" */
        temp = message.substr( 7 );
        temp = temp.substr( 0, temp.find( " " ));
        ui->print( " * %s %s", temp.c_str(),
                   message.substr( 8 + temp.size() ).c_str() );
    }
    else if( message.substr( 0, 6 ) == "rename" )
    {  /* someone selected a new nick */
        temp = message.substr( 7 );
        temp = temp.substr( 0, temp.find( " " ));
        ui->print( "(i) %s -> %s", temp.c_str(),
                   message.substr( 8 + temp.size() ).c_str() );
    }
    else
        ui->print( "UNKOWN: '%s'", message.c_str() );
}

void Protocol::msg( std::string message )
{
    net->send( std::string( "msg " ) + message );
}

void Protocol::cmd( std::string command )
{
    /* FIXME: make me do something! */
    net->send( command.erase( 0, 1 ) );
}
