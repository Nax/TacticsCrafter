#include <QtWidgets>
#include <TacticsCrafter/UI/TabBuild.h>
#include <TacticsCrafter/UI/FileSelector.h>

TabBuild::TabBuild(QWidget* parent)
: QWidget{parent}
{
    auto fileSelectorIn = new FileSelector;
    auto fileSelectorOut = new FileSelector;

    auto layout = new QVBoxLayout;
    layout->addWidget(fileSelectorIn);
    layout->addWidget(fileSelectorOut);
    setLayout(layout);
}
