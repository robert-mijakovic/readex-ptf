/**
   @file	peer_acceptor.cc
   @ingroup Communication
   @brief   Acceptor pattern implementation
   @author	Karl Fuerlinger
   @verbatim
        Revision:       $Revision$
        Revision date:  $Date$
        Committed by:   $Author$

        This file is part of the Periscope performance measurement tool.
        See http://www.lrr.in.tum.de/periscope for details.

        Copyright (c) 2005-2011, Technische Universitaet Muenchen, Germany
        See the COPYING file in the base directory of the package for details.
   @endverbatim
 */

#include <assert.h>

#include "ace/Auto_Ptr.h"
#include "ace/INET_Addr.h"
#include "ace/Reactor.h"
#include "ace/Log_Msg.h"

#include "psc_agent.h"
#include "peer_connection.h"
#include "peer_acceptor.h"

PeerAcceptor::~PeerAcceptor() {
    this->handle_close( ACE_INVALID_HANDLE, 0 );
}


int PeerAcceptor::handle_close( ACE_HANDLE       hdle,
                                ACE_Reactor_Mask maske ) {
    psc_dbgmsg( 7, "PeerAcceptor::handle_close\n" );
    if( acceptor_.get_handle() != ACE_INVALID_HANDLE ) {
        ACE_Reactor_Mask m =
            ACE_Event_Handler::ACCEPT_MASK |
            ACE_Event_Handler::DONT_CALL;

        assert( reactor() );
        reactor()->remove_handler( this, m );
        acceptor_.close();
    }
    return 0;
}


int PeerAcceptor::handle_input( ACE_HANDLE hdle ) {
    PeerConnection* conn;
    ACE_NEW_RETURN( conn, PeerConnection, -1 );

    unique_ptr<PeerConnection> p( conn );

    //ACE_DEBUG( (LM_DEBUG, "PeerAcceptor::handle_input\n") );

    if( acceptor_.accept( conn->peer() ) == -1 ) {
        ACE_ERROR_RETURN( ( LM_ERROR, "Failed to accept()" ),
                          -1 );
    }

    p.release();

    conn->reactor( reactor() );
    if( conn->open() == -1 ) {
        conn->handle_close( ACE_INVALID_HANDLE, 0 );
    }

    // todo: some error handling here!
    psc_dbgmsg( 7, "PeerAcceptor::handle_input: creating ACCL_Handler\n" );
    conn->set_protocol_handler( agent_->
                                create_protocol_handler( conn->peer() ) );


    return 0;
}


int PeerAcceptor::open( ACE_INET_Addr& listen_addr ) {
    if( acceptor_.open( listen_addr, 1 ) == -1 ) {
        return -1;
    }

    assert( reactor() );

    return reactor()->register_handler( this, ACE_Event_Handler::ACCEPT_MASK );
}


int PeerAcceptor::get_local_addr( ACE_INET_Addr& addr ) {
    psc_dbgmsg( 9, "PeerAcceptor::get_local_addr\n" );
    return acceptor_.get_local_addr( addr );
}
