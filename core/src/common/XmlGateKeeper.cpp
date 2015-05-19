
#include "swatch/core/XmlGateKeeper.hpp"
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

#include <fstream>
#include <iostream>

#include <xdata/Integer.h>
#include <xdata/String.h>
#include <xdata/Boolean.h>
#include <xdata/Float.h>


namespace swatch {
namespace core {


//   void printTree ( const boost::property_tree::ptree &pt, int level=0) {
//     
//     std::cout << "\""<< pt.data()<< "\"";
//     if ( !pt.empty() ) {
// 
//       if (level) std::cout << std::endl; 
// 
//       for ( boost::property_tree::ptree::const_iterator pos = pt.begin(); pos != pt.end() ; ) {
//         std::cout << std::string(level+1,' ') << "\"" << pos->first << "\": "; 
//         printTree(pos->second, level + 1); 
//         ++pos; 
//         if (pos != pt.end()) {
//           std::cout << ","; 
//         }
//         std::cout << std::endl;
//       } 
// 
//     }
//   }



  void FillRow( const boost::property_tree::ptree &aSrcRow , xdata::Table* aDestTable )
  {
    uint32_t lRowId( aSrcRow.get<uint32_t>("<xmlattr>.id") );

    const std::map< std::string, std::string , xdata::Table::ci_less>& lColumns( aDestTable->getTableDefinition() );

    BOOST_FOREACH( const boost::property_tree::ptree::value_type &lIt, aSrcRow ) {
      if( lIt.first == "entry" )
      {
        std::string lEntryId( lIt.second.get<std::string>("<xmlattr>.id") );
        std::string lEntryType( lIt.second.get<std::string>("<xmlattr>.type") );

        if( lColumns.find( lEntryId ) == lColumns.end() ) aDestTable->addColumn( lEntryId , lEntryType );

             if( lEntryType == "int" )   { xdata::Integer lVal(lIt.second.get<int>(""));        aDestTable->setValueAt( lRowId , lEntryId , lVal ); }
        else if( lEntryType == "bool" )  { xdata::Boolean lVal(lIt.second.get<bool>(""));       aDestTable->setValueAt( lRowId , lEntryId , lVal ); }
        else if( lEntryType == "float" ) { xdata::Float lVal(lIt.second.get<float>(""));        aDestTable->setValueAt( lRowId , lEntryId , lVal ); }
        else                             { xdata::String lVal(lIt.second.get<std::string>("")); aDestTable->setValueAt( lRowId , lEntryId , lVal ); }
      }
    }    
  }


  void FillTable( const boost::property_tree::ptree &aSrcTable , xdata::Table* aDestTable )
  {
    BOOST_FOREACH( const boost::property_tree::ptree::value_type &lIt, aSrcTable ) {
      if( lIt.first == "row" ) FillRow( lIt.second , aDestTable );
    }
  }




  XmlGateKeeper::XmlGateKeeper( Object* aToplevel , const std::string& aFileName , const uint32_t& aKey ):
    GateKeeper( aToplevel , aKey )
  {

    boost::property_tree::ptree lPtree;
    boost::property_tree::xml_parser::read_xml( swatch::core::shellExpandPath(aFileName) , lPtree );

//     printTree ( lPtree );

    const boost::property_tree::ptree& lDb( lPtree.get_child("db") );
    BOOST_FOREACH( const boost::property_tree::ptree::value_type &lIt, lDb ) {
      if( lIt.first == "table" )
      {
        std::string lId( lIt.second.get<std::string>("<xmlattr>.id") );
        xdata::Table* lTable = new xdata::Table();
        FillTable( lIt.second , lTable );
        add( lId , lTable );
      }
    }
  }

  XmlGateKeeper::~XmlGateKeeper()
  {}

  xdata::Table* XmlGateKeeper::getTable( const std::string& aId )
  {
    return NULL;
  }

} /* namespace core */
} /* namespace swatch */
