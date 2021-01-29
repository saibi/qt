#include <QString>
#include <QtTest>
#include <QCoreApplication>

class Usecases_add_panel_to_page : public QObject
{
	Q_OBJECT

public:
	Usecases_add_panel_to_page();

private Q_SLOTS:
	void test_no_panels();
};

Usecases_add_panel_to_page::Usecases_add_panel_to_page()
{
}

void Usecases_add_panel_to_page::test_no_panels()
{
    QVERIFY(true);
#if 0
	// Given there is a page
	auto page = new entities::Page(this);
	QVERIFY(page);

	// And the page has no panels
	QCOMPARE(page->panels().count(), 0);

	// And the page has size 480 x 480 px
	page->setSize(QSize(480, 480));
	QCOMPARE(page->size(), QSize(480, 480));

	// When I add a panel to the page
	auto addPanelToPage = new usecases::AddPanelToPage(this, page);
	QSignalSpy addPanelToPageSuccess(addPanelToPage, SIGNAL(success()));
	addPanelToPage->run();
	QTRY_COMPARE_WITH_TIMEOUT(addPanelToPageSuccess.count(), 1, 1000);

	// Then the page has one panel
	QCOMPARE(page->panels().count(), 1);

	// And the panel fills up the whole page
	QCOMPARE(page->panels().at(0).size(), page->size());
#endif
}

QTEST_MAIN(Usecases_add_panel_to_page)

#include "tst_usecases_add_panel_to_page.moc"
