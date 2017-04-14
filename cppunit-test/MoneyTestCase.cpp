// MoneyTest.cpp


#include <cppunit/config/SourcePrefix.h>
#include "Money.h"
#include "MoneyTestCase.h"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( MoneyTestCase );


void 
MoneyTestCase::setUp()
{
}


void 
MoneyTestCase::tearDown()
{
}


void 
MoneyTestCase::testConstructor()
{
  // Set up
  const std::string currencyFF( "FF" );
  const double longNumber = 1234.5678;

  // Process
  Money money( longNumber, currencyFF );

  // Check
  CPPUNIT_ASSERT_EQUAL( longNumber, money.getAmount() );
  CPPUNIT_ASSERT_EQUAL( currencyFF, money.getCurrency() );
}


void
MoneyTestCase::testEqual()
{
  // Set up
  const Money money123FF( 123, "FF" );
  const Money money123USD( 123, "USD" );
  const Money money12FF( 12, "FF" );
  const Money money12USD( 12, "USD" );

  // Process & Check
  CPPUNIT_ASSERT( money123FF == money123FF );    // ==
  CPPUNIT_ASSERT( money12FF != money123FF );     // != amount
  CPPUNIT_ASSERT( money123USD != money123FF );   // != currency
  CPPUNIT_ASSERT( money12USD != money123FF );    // != currency and != amount
}


void 
MoneyTestCase::testAdd()
{
  // Set up
  const Money money12FF( 12, "FF" );
  const Money expectedMoney( 135, "FF" );

  // Process
  Money money( 123, "FF" );
  money += money12FF;

  // Check
  CPPUNIT_ASSERT_EQUAL( expectedMoney, money );           // add works
  CPPUNIT_ASSERT( &money == &(money += money12FF) );  // add returns ref. on 'this'.
}


void 
MoneyTestCase::testAddThrow()
{
  // Set up
  const Money money123FF( 123, "FF" );

  // Process
  Money money( 123, "USD" );
  money += money123FF;        // should throw an exception
}
