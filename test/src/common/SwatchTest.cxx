/* 
 * File:   SystemTesrt.cpp
 * Author: ale
 *
 * Created on July 8, 2014, 2:43 PM
 */

#include <cstdlib>

#include "swatch/test/TestSuite.hpp"
 
// C++ Headers
#include <stdexcept>
#include <iostream>
#include <vector>

// Boost Headers
#include <boost/program_options.hpp>
#include <boost/foreach.hpp>

using namespace std;

//namespace po = boost::program_options;

/*
 * 
 */
int main(int argc, char** argv) {
    using namespace swatch::test;
    
    set<string> args(argv+1,argv+argc);
    BOOST_FOREACH( string a, args) {
        cout << a << endl;
    }
    
//    vector<swatch::test::TestUnit*> units;
    
    
    map<string, TestUnit*>  units;
//    units["sys_build"]   = new SystemBuildTest();
    units["sys_explore"] = new SystemExploreTest();
//    units["sys_crate"]   = new SystemBuildFullCrate();
//    units["par_test"]    = new ParameterSetTest();
    units["json_build"]  = new JsonBuildTest();

    int counts = 0;
    BOOST_FOREACH(string a, args) {

        try {

            if ( units.count( a ) ) {
                ++counts;
                units[a]->run();
            }

        } catch (const runtime_error e) {
            cout << "ERROR: " << e.what() << endl;
        }

    }

    if ( !counts ) {
        cout  << "WARNING: No test executed" << endl;
        cout  << "Available tests are:" << endl;
        map<string, TestUnit*>::const_iterator it;
        for ( it = units.begin(); it != units.end(); ++it )
            cout << " - " << it->first << endl;
    }

    // Cleanup
    map<string, TestUnit*>::iterator itU;
    for( itU = units.begin(); itU != units.end(); ++itU)
        delete itU->second;
    
}

