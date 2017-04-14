/***********************************************************
 *
 * Header file MSVCOutputter.h for class MSVCOutputter
 * (c)Copyright 2015, David Dibben
 *
 **********************************************************/

#ifndef MSVC_OUTPUTTER_H
#define MSVC_OUTPUTTER_H

#include <cppunit/Portability.h>
#include <cppunit/Outputter.h>
#include <cppunit/portability/Stream.h>

CPPUNIT_NS_BEGIN
class Exception;
class SourceLine;
class Test;
class TestFailure;
class TestResultCollector;
CPPUNIT_NS_END

/*!
   \brief A test result printer in an MSVC like format.
*/

class MSVCOutputter : public CPPUNIT_NS::Outputter
{
public:

  MSVCOutputter( CPPUNIT_NS::TestResultCollector *result, CPPUNIT_NS::OStream& stream );
  ~MSVCOutputter();

  static MSVCOutputter *defaultOutputter(CPPUNIT_NS::TestResultCollector* result, CPPUNIT_NS::OStream& stream);

  void write();

  virtual void printSuccess();
  virtual void printFailureReport();
  virtual void printFailuresList();
  virtual void printStatistics();
  virtual void printFailureDetail(CPPUNIT_NS::TestFailure* failure );
  virtual void printFailureLocation(CPPUNIT_NS::SourceLine sourceLine );
  virtual void printFailureType(CPPUNIT_NS::TestFailure* failure );
  virtual void printFailedTestName(CPPUNIT_NS::TestFailure* failure );
  virtual void printFailureMessage(CPPUNIT_NS::TestFailure* failure );

private:

  MSVCOutputter(const MSVCOutputter& copy);
  void operator=(const MSVCOutputter& copy);

  virtual bool processLocationFormatCommand(char command, const CPPUNIT_NS::SourceLine& sourceLine);

  virtual std::string extractBaseName(const std::string& fileName ) const;

private:

  CPPUNIT_NS::TestResultCollector*  m_result;
  CPPUNIT_NS::OStream&				m_stream;
  std::string						m_locationFormat;

};



#endif
