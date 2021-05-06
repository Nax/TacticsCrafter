#include <QtWidgets>
#include <TacticsCrafter/UI/TabBuild.h>
#include <TacticsCrafter/UI/FileSelector.h>

TabBuild::TabBuild(QWidget* parent)
: QWidget{parent}
{
    auto fileSelectorIn = new FileSelector;
    fileSelectorIn->setLabel("Clean ISO");
    fileSelectorIn->setDialogOptions("Open Source ISO", "ISO image file (*.iso)");

    auto fileSelectorOut = new FileSelector;
    fileSelectorOut->setLabel("Destination ISO");
    fileSelectorOut->setDialogOptions("Destination ISO", "ISO image file (*.iso)", true);

    auto layout = new QVBoxLayout;
    layout->addStretch(1);
    layout->addWidget(fileSelectorIn);
    layout->addWidget(fileSelectorOut);
    layout->addStretch(1);

    auto topLayout = new QHBoxLayout;
    topLayout->addStretch(1);
    topLayout->addLayout(layout);
    topLayout->addStretch(1);
    setLayout(topLayout);
}
