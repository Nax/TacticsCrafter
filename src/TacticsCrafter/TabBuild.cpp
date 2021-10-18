#include <libtactics/tactics.h>
#include <QtWidgets>
#include <TacticsCrafter/TabBuild.h>
#include <TacticsCrafter/FileSelector.h>

namespace
{

void buildCallback(void* arg, int progress)
{
    TabBuild* tab = (TabBuild*)arg;
    tab->report(progress);
}

}

TabBuild::TabBuild(LTC_Context* ctx, QWidget* parent)
: QWidget{parent}
, _ctx{ctx}
{
    auto fileSelectorIn = new FileSelector;
    fileSelectorIn->setLabel("Clean ISO");
    fileSelectorIn->setDialogOptions("Open Source ISO", "ISO image file (*.iso)");

    auto fileSelectorOut = new FileSelector;
    fileSelectorOut->setLabel("Destination ISO");
    fileSelectorOut->setDialogOptions("Destination ISO", "ISO image file (*.iso)", true);

    auto button = new QPushButton("Build");

    _progress = new QProgressBar;
    _progress->setRange(0, 1000);

    auto layout = new QVBoxLayout;
    layout->addStretch(1);
    layout->addWidget(fileSelectorIn);
    layout->addWidget(fileSelectorOut);
    layout->addWidget(button);
    layout->addWidget(_progress);
    layout->addStretch(1);

    auto topLayout = new QHBoxLayout;
    topLayout->addStretch(1);
    topLayout->addLayout(layout);
    topLayout->addStretch(1);
    setLayout(topLayout);

    connect(fileSelectorIn, &FileSelector::selected, [this](QString path) { _fileIn = path; });
    connect(fileSelectorOut, &FileSelector::selected, [this](QString path) { _fileOut = path; });
    connect(button, &QPushButton::pressed, this, &TabBuild::run);
}

void TabBuild::report(int number)
{
    _progress->setValue(number);
}

void TabBuild::run()
{
    _progress->reset();
    ltcBuildImage(_ctx, _fileOut.toStdString().c_str(), _fileIn.toStdString().c_str(), &buildCallback, this);
}
