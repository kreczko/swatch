
#include "swatch/core/XmlGateKeeper.hpp"


#include <fstream>
#include <iostream>

#include <boost/lexical_cast.hpp>

#include <xdata/Integer.h>
#include <xdata/String.h>
#include <xdata/Boolean.h>
#include <xdata/Float.h>

#include "pugixml/pugixml.hpp"


#include "swatch/logger/Log.hpp"
#include "swatch/core/Utilities.hpp"


namespace swatch {
namespace core {

//------------------------------------------------------------------------------------------------------------------
  XmlGateKeeper::XmlGateKeeper( Object* aToplevel , const std::string& aFileName , const std::string& aKey ):
    GateKeeper( aToplevel , aKey ),
    mFileName( swatch::core::shellExpandPath(aFileName) )
  {

    pugi::xml_document lXmlDoc;
    pugi::xml_parse_result lLoadResult = lXmlDoc.load_file( mFileName.c_str() );

    if ( not lLoadResult )
    {
      LOG(swatch::logger::kError) << "Error reading XML file '" << aFileName << "'. Details: " << lLoadResult.description() << std::endl;
      throw XmlFileError("Error reading XML file '" + aFileName + "' : " + lLoadResult.description());
    }

    pugi::xml_node lRun( lXmlDoc.child("db").find_child_by_attribute( "run", "key", aKey.c_str() ) );

    for (pugi::xml_node lTable( lRun.child("table") ); lTable ; lTable = lTable.next_sibling("table") )
    {
      std::pair< std::string , GateKeeper::tTable > lParameterTable( CreateTable( lTable ) );
      add( lParameterTable.first , lParameterTable.second );
    }
  }
//------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------
  XmlGateKeeper::~XmlGateKeeper()
  {}
//------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------
  std::pair< std::string , GateKeeper::tParameter > XmlGateKeeper::CreateParameter( pugi::xml_node& aEntry )
  {
    std::string lEntryId( aEntry.attribute("id").value() );
    std::string lType( aEntry.attribute("type").value() );
    std::string lValue( aEntry.child_value() );

    // GateKeeper::tParameter lParameter;
    xdata::Serializable* lSerializable(0x0);

         if( lType == "int" )    { lSerializable = new xdata::Integer( ); }
    else if( lType == "bool" )   { lSerializable = new xdata::Boolean( ); }
    else if( lType == "float" )  { lSerializable = new xdata::Float  ( ); }
    else if( lType == "string" ) { lSerializable = new xdata::String ( ); }
    else { throw UnknownDataType( "Unknown Data-Type '" + lType + "'" ); }

    // exploit fromString to import the value inn the right format (use xdaq::compliant representation)
    lSerializable->fromString(lValue);

    // lParameter = GateKeeper::tParameter(lSerializable);
    // return std::make_pair( lEntryId , lParameter );
    return std::make_pair( lEntryId , GateKeeper::tParameter(lSerializable) );
    
  }
//------------------------------------------------------------------------------------------------------------------
  
//------------------------------------------------------------------------------------------------------------------
  std::pair< std::string , GateKeeper::tTable > XmlGateKeeper::CreateTable( pugi::xml_node& aTable )
  {
    std::string lTableId( aTable.attribute("id").value() );
    tTable lParameterTable( new tParameters() );

    for (pugi::xml_node lEntry( aTable.child("entry") ); lEntry ; lEntry = lEntry.next_sibling("entry") )
    {
      std::pair< std::string , GateKeeper::tParameter > lParameter( CreateParameter( lEntry ) );

      if( lParameterTable->find( lParameter.first ) != lParameterTable->end() )
      {
        throw ParameterWithGivenIdAlreadyExistsInTable( "Parameter with ID '" + lParameter.first + "' already exists in table '" + lTableId + "' in file '" + mFileName + "'" );
      }

      lParameterTable->insert( lParameter );
    }
    return std::make_pair( lTableId , lParameterTable );
  }
//------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------
  GateKeeper::tTable XmlGateKeeper::getTable( const std::string& aKey , const std::string& aId )
  {
    return GateKeeper::tTable();
  }
//------------------------------------------------------------------------------------------------------------------

} /* namespace core */
} /* namespace swatch */
