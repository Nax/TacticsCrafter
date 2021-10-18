#include <QtWidgets>
#include <TacticsCrafter/FileSelector.h>

FileSelector::FileSelector(QWidget* parent)
: QWidget{parent}
, _dialogAllowNewFile{}
{
    auto layout = new QHBoxLayout;

    _label = new QLabel("File");
    _label->setMinimumWidth(100);

    _lineEdit = new QLineEdit;
    _lineEdit->setMinimumWidth(300);

    _button = new QPushButton("...");

    layout->addWidget(_label);
    layout->addWidget(_lineEdit, 1);
    layout->addWidget(_button);

    setLayout(layout);

    connect(_button, &QPushButton::pressed, this, &FileSelector::selectFile);
    connect(_lineEdit, &QLineEdit::textChanged, this, &FileSelector::selected);
}

void FileSelector::setLabel(const QString label)
{
    _label->setText(label);
}

void FileSelector::setDialogOptions(const QString& title, const QString& filter, bool allowNew)
{
    _dialogTitle = title;
    _dialogFilter = filter;
    _dialogAllowNewFile = allowNew;
}

void FileSelector::selectFile()
{
    QString filename;

    if (!_dialogAllowNewFile)
        filename = QFileDialog::getOpenFileName(this, _dialogTitle, "", _dialogFilter);
    else
        filename = QFileDialog::getSaveFileName(this, _dialogTitle, "", _dialogFilter);

    _lineEdit->setText(filename);

    emit selected(filename);
}
