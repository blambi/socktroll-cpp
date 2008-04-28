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
        ui->print( "<" + temp + "> " + message.substr( 5 + temp.size() ) );
    }
    else if( message.substr( 0, 2 ) == "no" )
    { /* Nick change refusals */
        msg_ok = true;

        /* diffrent reasons */
        if( message.substr( 3 ) == "taken" )
            ui->print( " ! The nick is already in use" );
        else if( message.substr( 3 ) == "bad" )
            ui->print( " ! Not a valid nick" );
        else
            ui->print( " ! Unknown reason " + message.substr( 3 ) );
    }

    if( ! auth_ok )
    {
        if( message.substr( 0, 2 ) == "ok" )
        {
            /* 
               Maybe we should add a check if we really got our
               selected nick?
            */
            ui->print( " + " + message.substr( 3 ) );
            nick = message.substr( 3 );
            auth_ok = true;
            msg_ok = true;
        }
        else if( message == "illegal command" )
            ui->print( " ! You must set a nick first" );
        else if( ! msg_ok )
        {
            ui->print( "UNKNOWN: '" + message + "'" );
        }
    }
    else
    {
        if( message.substr( 0, 1 ) == "+" or
                 message.substr( 0, 1 ) == "-" )
        { /* join / part ex: + foo */
            ui->print( " " + message.substr( 0, 1 ) + " " +
                       message.substr( 2 ) );
        }
        else if( message.substr( 0, 6 ) == "action" )
        {  /* "/me" */
            temp = message.substr( 7 );
            temp = temp.substr( 0, temp.find( " " ));
            ui->print( " * " + temp + " " +
                       message.substr( 8 + temp.size() ) );
        }
        else if( message.substr( 0, 6 ) == "rename" )
        {  /* someone selected a new nick */
            temp = message.substr( 7 );
            temp = temp.substr( 0, temp.find( " " ));
            nick = message.substr( 8 + temp.size() );
            ui->print( " i " + temp + " -> " + nick );
        }
        else if( message == "illegal command" )
        {
            ui->print( " ! illegal command" );
        }
        else if( ! msg_ok )
            ui->print( "UNKOWN: '" + message + "'" );
    }
}

void Protocol::msg( std::string message )
{
    if( auth_ok )
        net->send( std::string( "msg " ) + message );
    else
        ui->print( "You must first auth with /nick" );
}

void Protocol::cmd( std::string command )
{
    bool cmd_done;
    
    /* Always valid */
    if( command.substr( 0, 4 ) == "nick" )
    {
        net->send( "nick " + command.substr( 5 ) );
        cmd_done = true;
    }

    /* Must auth first */
    if( auth_ok )
    {
        if( command.substr( 0, 2 ) == "me" )
        {
            net->send( "action " + command.substr( 3 ) );
            cmd_done = true;
        }
        else if( command.substr( 0, 6 ) == "action" )
        {
            net->send( "action " + command.substr( 7 ) );
            cmd_done = true;
        }
        else if( command.substr( 0, 5 ) == "names" )
        {
            net->send( "names" );
            cmd_done = true;
        }
    }
    
    if( ! cmd_done )
    {
        /* Should only be unknowns here */
        if( auth_ok )
            ui->print( "*** Unknown command" );
        else
            ui->print( "*** Not available" );
    }
}

void Protocol::auth( void )
{
    std::string t_nick;

    if( ! auth_ok )
    {
        ui->print( "*** Select your nick:" );
        while( t_nick == "" )
            t_nick = ui->input();
        this->cmd( std::string( "nick " ) + t_nick );
    } /* else its just a ghost call */
}
