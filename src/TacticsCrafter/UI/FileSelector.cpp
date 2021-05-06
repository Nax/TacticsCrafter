#include <QtWidgets>
#include <TacticsCrafter/UI/FileSelector.h>

FileSelector::FileSelector(QWidget* parent)
: QWidget{parent}
{
    auto layout = new QHBoxLayout;

    _label = new QLabel("File");
    _lineEdit = new QLineEdit;
    _lineEdit->setMinimumWidth(300);
    _button = new QPushButton("...");

    layout->addWidget(_label);
    layout->addWidget(_lineEdit, 1);
    layout->addWidget(_button);

    setLayout(layout);
}
