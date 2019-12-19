#include "master-controller-tests.h"

namespace cm {
namespace controllers {


static MasterControllerTests instance;

MasterControllerTests::MasterControllerTests() : TestSuite("MasterControllerTests")
{

}

}

namespace controllers {

void MasterControllerTests::initTestCase()
{

}

void MasterControllerTests::cleanupTestCase()
{

}

void MasterControllerTests::init()
{

}

void MasterControllerTests::cleanup()
{

}

}

namespace controllers {

void MasterControllerTests::welcomeMessage_returnsCorrectMessage()
{
	QCOMPARE( masterController.welcomeMessage(), QString("Welcome to the Client Management System!") );
}

}
}
