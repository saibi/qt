/***********************************************************
 *
 * Header file GccOutputter.h for class GccOutputter
 * (c)Copyright 2015, David Dibben
 *
 **********************************************************/

#ifndef GCC_OUTPUTTER_H
#define GCC_OUTPUTTER_H

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
   \brief A test result printer in an GCC like format.
*/

class GccOutputter : public CPPUNIT_NS::Outputter
{
public:

  GccOutputter(CPPUNIT_NS::TestResultCollector* result, CPPUNIT_NS::OStream& stream);
  ~GccOutputter();


  static GccOutputter* defaultOutputter(CPPUNIT_NS::TestResultCollector* result, CPPUNIT_NS::OStream& stream );

  void write();

  virtual void printSuccess();
  virtual void printFailureReport();
  virtual void printFailuresList();
  virtual void printStatistics();
  virtual void printFailureDetail(CPPUNIT_NS::TestFailure* failure);
  virtual void printFailureLocation(CPPUNIT_NS::SourceLine sourceLine);
  virtual void printFailureType(CPPUNIT_NS::TestFailure* failure);
  virtual void printFailedTestName(CPPUNIT_NS::TestFailure* failure);
  virtual void printFailureMessage(CPPUNIT_NS::TestFailure* failure);

private:

  GccOutputter(const GccOutputter& copy);
  void operator=(const GccOutputter& copy);

  virtual bool processLocationFormatCommand(char command, const CPPUNIT_NS::SourceLine& sourceLine);

  virtual std::string extractBaseName(const std::string& fileName) const;

private:

  CPPUNIT_NS::TestResultCollector*  m_result;
  CPPUNIT_NS::OStream&				m_stream;
  std::string						m_locationFormat;

};


#endif
