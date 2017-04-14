/***********************************************************
 *
 * Implementation file MSVCOutputter.cpp for class MSVCOutputter
 * (c)Copyright 2015, David Dibben
 *
 * Based on CompilerOutputter from CppUnit
 *
 **********************************************************/

#include "MSVCOutputter.h"

#include <cppunit/config/SourcePrefix.h>
#include <cppunit/Exception.h>
#include <cppunit/SourceLine.h>

#include <cppunit/TestFailure.h>
#include <cppunit/TestResultCollector.h>

#include <algorithm>
#include <cppunit/tools/StringTools.h>

using namespace CPPUNIT_NS;

MSVCOutputter::MSVCOutputter(TestResultCollector* result, OStream& stream ) :
	m_result( result ),
	m_stream( stream ),
	m_locationFormat( "%p(%l) : " )
{
}


MSVCOutputter::~MSVCOutputter()
{
}


MSVCOutputter* MSVCOutputter::defaultOutputter(TestResultCollector* result, OStream& stream )
{
	return new MSVCOutputter(result, stream);
}


void MSVCOutputter::write()
{
	if (m_result->wasSuccessful()) {
		printSuccess();
	} else {
		printFailureReport();
	}
}


void MSVCOutputter::printSuccess()
{
	m_stream  << "OK (" << m_result->runTests()  << ")\n";
}


void MSVCOutputter::printFailureReport()
{
	printFailuresList();
	printStatistics();
}


void MSVCOutputter::printFailuresList()
{
	for (int index =0; index < m_result->testFailuresTotal(); ++index) {
		printFailureDetail( m_result->failures()[ index ] );
	}
}


void MSVCOutputter::printFailureDetail(TestFailure* failure)
{
	printFailureLocation( failure->sourceLine() );
	printFailureType( failure );
	printFailedTestName( failure );
	printFailureMessage( failure );
}


void MSVCOutputter::printFailureLocation(SourceLine sourceLine)
{
	if (!sourceLine.isValid() ) {
		m_stream  <<  "##Failure Location unknown## : ";
		return;
	}

	for (unsigned int index = 0; index < m_locationFormat.length(); ++index) {
		char c = m_locationFormat[ index ];
		if ( c == '%'  &&  ( index+1 < m_locationFormat.length() ) ) {
			char command = m_locationFormat[index+1];
			if (processLocationFormatCommand( command, sourceLine ) ) {
				++index;
				continue;
			}
		}

		m_stream  << c;
	}
}


bool MSVCOutputter::processLocationFormatCommand(char command, const SourceLine& sourceLine )
{
	switch ( command ) {
		case 'p':
			m_stream  <<  sourceLine.fileName();
			return true;
		case 'l':
			m_stream  <<  sourceLine.lineNumber();
			return true;
		case 'f':
			m_stream  <<  extractBaseName( sourceLine.fileName() );
			return true;
	}

	return false;
}


std::string MSVCOutputter::extractBaseName(const std::string& fileName) const
{
	int indexLastDirectorySeparator = fileName.find_last_of( '/' );

	if ( indexLastDirectorySeparator < 0 ) {
		indexLastDirectorySeparator = fileName.find_last_of( '\\' );
	}

	if ( indexLastDirectorySeparator < 0 ) {
		return fileName;
	}

	return fileName.substr( indexLastDirectorySeparator +1 );
}


void MSVCOutputter::printFailureType(TestFailure* failure )
{
	m_stream  <<  (failure->isError() ? "fatal error T9999" : "error T9999");
}


void MSVCOutputter::printFailedTestName(TestFailure* failure )
{
	m_stream  <<  ": "  <<  failure->failedTestName();
}


void MSVCOutputter::printFailureMessage(TestFailure* failure )
{
	Exception *thrownException = failure->thrownException();
	m_stream  <<"        "<< thrownException->message().shortDescription()  <<  "\n";

	std::string message = thrownException->message().details();

	StringTools::Strings list = StringTools::split( message, '\n' );

	for (CppUnitVector<std::string>::iterator it = list.begin(); it != list.end(); ++it) {
		std::string line = *it;
		size_t pos = line.find("  :");
		if (pos != std::string::npos) {
			line.replace(pos, 2, ":");
		}

		m_stream <<"        "<<line<<"\n";
	}
}


void MSVCOutputter::printStatistics()
{
	m_stream  <<  "Failures !!!\n";
	m_stream  <<  "Run: "  <<  m_result->runTests()  << "   "
			  <<  "Failure total: "  <<  m_result->testFailuresTotal()  << "   "
			  <<  "Failures: "  <<  m_result->testFailures()  << "   "
			  <<  "Errors: "  <<  m_result->testErrors()
			  <<  "\n";
}



