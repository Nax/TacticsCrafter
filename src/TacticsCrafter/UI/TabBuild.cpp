#include <QtWidgets>
#include <TacticsCrafter/UI/TabBuild.h>
#include <TacticsCrafter/UI/FileSelector.h>
#include <TacticsCrafter/Core/ImageBuilder.h>
#include <TacticsCrafter/Core/ScriptManager.h>

TabBuild::TabBuild(ScriptManager& scripts, QWidget* parent)
: QWidget{parent}
, _scripts{scripts}
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

void TabBuild::run()
{
    ImageBuilder builder;
    _progress->reset();

    builder.setInput(_fileIn.toStdString().c_str());
    builder.setOutput(_fileOut.toStdString().c_str());

    connect(&builder, &ImageBuilder::progress, _progress, &QProgressBar::setValue);

    builder.apply(_scripts.run());
    builder.finish();
}
