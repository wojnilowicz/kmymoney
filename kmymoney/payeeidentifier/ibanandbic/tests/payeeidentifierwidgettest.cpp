#include "payeeidentifierwidgettest.h"

#include "../widgets/ibanbicitemedit.h"
#include "../widgets/kbankcodeedit.h"
#include "../widgets/kbicedit.h"
#include "../widgets/kibanlineedit.h"

#include <QApplication>
#include <QGridLayout>
#include <QLabel>

PayeeIdentifierWidgetTest::PayeeIdentifierWidgetTest()
{
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(new QLabel("KBicEdit"), 0, 0);
    layout->addWidget(new KBicEdit(this), 0, 1);
    layout->addWidget(new QLabel("KIbanLineEdit"), 1, 0);
    layout->addWidget(new KIbanLineEdit(this), 1, 1);
    layout->addWidget(new QLabel("KBankCodeEdit"), 2, 0);
    layout->addWidget(new KBankCodeEdit(this), 2, 1);
    layout->addWidget(new QLabel("ibanbicitemedit"), 3, 0);
    layout->addWidget(new ibanBicItemEdit(this), 3, 1);
    setLayout(layout);
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    PayeeIdentifierWidgetTest test;
    test.exec();
}
