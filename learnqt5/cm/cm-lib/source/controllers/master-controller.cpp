#include "master-controller.h"

#include <models/client.h>

namespace cm {
namespace controllers {

class MasterController::Implementation
{
public:
		Implementation(MasterController* _masterController)
				: masterController(_masterController)
		{
				commandController = new CommandController(masterController);
				navigationController = new NavigationController(masterController);
				newClient = new models::Client(masterController);
		}

		MasterController* masterController{nullptr};
		CommandController* commandController{nullptr};
		NavigationController* navigationController{nullptr};
		QString welcomeMessage = "This is MasterController to Major Tom";
		models::Client * newClient{nullptr};
};

MasterController::MasterController(QObject* parent)
		: QObject(parent)
{
		implementation.reset(new Implementation(this));
}

MasterController::~MasterController()
{
}

CommandController* MasterController::commandController()
{
		return implementation->commandController;
}

NavigationController* MasterController::navigationController()
{
		return implementation->navigationController;
}

const QString& MasterController::welcomeMessage() const
{
		return implementation->welcomeMessage;
}

models::Client * MasterController::newClient()
{
	return implementation->newClient;
}

}}
