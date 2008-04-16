#include <string>
#include "socktroll.hpp"

Protocol::Protocol( void )
{
    nick = std::string( "" );
    auth_ok = false;
}

Protocol::~Protocol( void )
{
    if( net and auth_ok )
        net->send( "quit" );
}

void Protocol::parse( std::string message )
{
    bool msg_ok;
    std::string temp;

    /* in both modes */
    if( message.substr( 0, 3 ) == "msg" )
    {  /* normal message */
        msg_ok = true; /* to not get UNKNOWN messages */
        temp = message.substr( 4 );
        temp = temp.substr( 0, temp.find( " " ));
        ui->print( "<%s> %s", temp.c_str(),
                   message.substr( 5 + temp.size() ).c_str() );
    }

    if( ! auth_ok )
    {
        if( message.substr( 0, 2 ) == "ok" )
        {
            /* 
               Maybe we should add a check if we really got our
               selected nick?
            */
            ui->print( " + %s", message.substr( 3 ).c_str() );
            auth_ok = true;
            msg_ok = true;
        }
        if( message.substr( 0, 2 ) == "no" )
        {
            /* diffrent reasons */
            if( message.substr( 3 ) == "taken" )
                ui->print( " ! The nick is already in use" );
            else if( message.substr( 3 ) == "bad" )
                ui->print( " ! Not a valid nick" );
            else
                ui->print( " ! Unknown reason %s",
                           message.substr( 3 ).c_str() );
        }
        else if( message == "illegal command" )
            ui->print( " ! You must set a nick first" );
        else if( ! msg_ok )
        {
            ui->print( "UNKNOWN: '%s'", message.c_str() );
        }

        //if( ! auth_ok ) /* Continue with login attempts */
        //    this->auth();
        
    }
    else
    {
        if( message.substr( 0, 1 ) == "+" or
                 message.substr( 0, 1 ) == "-" )
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
            ui->print( " i %s -> %s", temp.c_str(),
                       message.substr( 8 + temp.size() ).c_str() );
        }
        else if( message == "illegal command" )
        {
            ui->print( " ! illegal command" );
        }
        else if( ! msg_ok )
            ui->print( "UNKOWN: '%s'", message.c_str() );
    }
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

void Protocol::auth( void )
{
    if( ! auth_ok )
    {
        ui->print( "Select your nick:" );
        while( nick == "" )
            nick = ui->input();
        this->cmd( std::string( "/nick " ) + nick );
    } /* else its just a ghost call */
}
