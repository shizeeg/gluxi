/*
  Copyright (c) 2006-2008 by Jakob Schroeter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/


#include "myvcardmanager.h"
#include "myvcardhandler.h"
#include <gloox/vcard.h>
#include <gloox/clientbase.h>
#include <gloox/disco.h>

namespace gloox
{

  MyVCardManager::MyVCardManager( ClientBase *parent )
    : m_parent( parent )
  {
    if( m_parent )
    {
      m_parent->registerIqHandler( this, XMLNS_VCARD_TEMP );
      m_parent->disco()->addFeature( XMLNS_VCARD_TEMP );
    }
  }

  MyVCardManager::~MyVCardManager()
  {
    if( m_parent )
    {
      m_parent->disco()->removeFeature( XMLNS_VCARD_TEMP );
      m_parent->removeIqHandler( XMLNS_VCARD_TEMP );
      m_parent->removeIDHandler( this );
    }
  }

  std::string MyVCardManager::fetchVCard( const JID& jid, MyVCardHandler *vch )
  {
    if( !m_parent || !vch )
      return "";

    TrackMap::const_iterator it = m_trackMap.find( jid.full() );
    if( it != m_trackMap.end() )
      return "";

    const std::string& id = m_parent->getID();
    Tag *iq = new Tag( "iq" );
    iq->addAttribute( "type", "get" );
    iq->addAttribute( "id", id );
    iq->addAttribute( "to", jid.full() );
    Tag *v = new Tag( iq, "vCard" );
    v->addAttribute( "xmlns", XMLNS_VCARD_TEMP );

    m_parent->trackID( this, id, MyVCardHandler::FetchVCard );
    m_trackMap[id] = vch;
    m_parent->send( iq );
    return id;
  }

  void MyVCardManager::cancelVCardOperations( MyVCardHandler *vch )
  {
    TrackMap::iterator t;
    TrackMap::iterator it = m_trackMap.begin();
    while( it != m_trackMap.end() )
    {
      t = it;
      ++it;
      if( (*t).second == vch )
        m_trackMap.erase( t );
    }
  }

  void MyVCardManager::storeVCard( const VCard *vcard,  MyVCardHandler *vch )
  {
    if( !m_parent || !vch )
      return;

    const std::string& id = m_parent->getID();
    Tag *iq = new Tag( "iq" );
    iq->addAttribute( "type", "set" );
    iq->addAttribute( "id", id );
    iq->addChild( vcard->tag() );

    m_parent->trackID( this, id, MyVCardHandler::StoreVCard );
    m_trackMap[id] = vch;
    m_parent->send( iq );
  }

  bool MyVCardManager::handleIq( Stanza * /*stanza*/ )
  {
    return false;
  }

  bool MyVCardManager::handleIqID( Stanza *stanza, int context )
  {
	  std::string id=stanza->id();
    TrackMap::iterator it = m_trackMap.find( stanza->id() );
    if( it != m_trackMap.end() )
    {
      switch( stanza->subtype() )
      {
        case StanzaIqResult:
        {
          switch( context )
          {
            case MyVCardHandler::FetchVCard:
            {
              Tag *v = stanza->findChild( "vCard", "xmlns", XMLNS_VCARD_TEMP );
              if( v ) {
            	//TODO: original gloox VCardManager do NOT remove VCard instance here after
            	//passing it to handler. Looks like bug.
            	VCard* vcard = new VCard( v );
                (*it).second->handleVCard(id, stanza->from(), vcard );
                delete vcard;
              } else
                (*it).second->handleVCard(id, stanza->from(), 0 );
              break;
            }
            case MyVCardHandler::StoreVCard:
              (*it).second->handleVCardResult(id, MyVCardHandler::StoreVCard, stanza->from() );
              break;
          }
        }
        break;
        case StanzaIqError:
        {
          switch( context )
          {
            case MyVCardHandler::FetchVCard:
              (*it).second->handleVCardResult(id, MyVCardHandler::FetchVCard, stanza->from(), stanza->error() );
              break;
            case MyVCardHandler::StoreVCard:
              (*it).second->handleVCardResult(id, MyVCardHandler::StoreVCard, stanza->from(), stanza->error() );
              break;
          }
          break;
        }
        default:
          return false;
      }

      m_trackMap.erase( it );
    }
    return false;
  }

}
