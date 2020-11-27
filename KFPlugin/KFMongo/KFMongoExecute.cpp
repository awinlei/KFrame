﻿#include "KFMongoExecute.hpp"

namespace KFrame
{
    /////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////
    void KFMongoExecute::InitMongo( const KFMongoConnectOption* connectoption, const KFMongoConnnectData* connectdata )
    {
        return KFMongo::InitMongo( connectoption, connectdata );
    }

    bool KFMongoExecute::IsIndexCreate( const std::string& table, const std::string& indexname )
    {
        auto iter = _table_index_list.find( table );
        if ( iter == _table_index_list.end() )
        {
            return false;
        }

        return iter->second.find( indexname ) != iter->second.end();
    }

    bool KFMongoExecute::CreateIndex( const std::string& table, const std::string& indexname, const MongoIndexType& values, bool unique, uint32 ttl )
    {
        if ( IsIndexCreate( table, indexname ) )
        {
            return true;
        }

        try
        {
            Poco::MongoDB::Document::Ptr temp = new Poco::MongoDB::Document();
            for ( auto& iter : values )
            {
                temp->add( iter.first, iter.second );
            }

            Poco::MongoDB::Database db( _database );
            auto newindexname = __FORMAT__( "idx_{}", indexname );
            auto doc = db.ensureIndex( *_connection, table, newindexname, temp, unique, false, 0, ttl );
            bool err = doc->get( "err" ).isNull();
            if ( !err )
            {
                return true;
            }
            __LOG_ERROR__( "mongo create index failed=[{}]", doc->toString() );
        }
        catch ( Poco::Exception& ex )
        {
            __LOG_ERROR__( "mongo create index error=[{}]", ex.displayText() );
        }

        return false;
    }

    void KFMongoExecute::AddPocoDocument( Document& pocodocument, const KFMongoSelector& kfselector )
    {
        if ( kfselector._key != 0u )
        {
            pocodocument.add( MongoKeyword::_id, kfselector._key );
        }

        auto kfdocument = &kfselector._document;
        if ( !kfdocument->_int_expressions.empty() ||
                !kfdocument->_str_expressions.empty() ||
                !kfdocument->_documents.empty() )
        {
            Poco::MongoDB::Array::Ptr array = new Poco::MongoDB::Array();
            AddPocoDocument( *array, kfdocument );
            pocodocument.add( kfdocument->_condition, array );
        }
    }

    void KFMongoExecute::AddPocoDocument( Document& pocodocument, const KFMongoDocument* kfdocument )
    {
        for ( auto expression : kfdocument->_int_expressions )
        {
            Poco::MongoDB::Document::Ptr temp = new Poco::MongoDB::Document();
            AddDocumentValue( *temp, expression );
            pocodocument.add( __TO_STRING__( pocodocument.size() ), temp );
        }

        for ( auto expression : kfdocument->_str_expressions )
        {
            Poco::MongoDB::Document::Ptr temp = new Poco::MongoDB::Document();
            AddDocumentValue( *temp, expression );
            pocodocument.add( __TO_STRING__( pocodocument.size() ), temp );
        }

        for ( auto document : kfdocument->_documents )
        {
            AddPocoDocument( pocodocument, document );
        }
    }
}
