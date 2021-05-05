#include <QtWidgets>
#include <TacticsCrafter/Core/Script.h>
#include <TacticsCrafter/UI/ScriptView.h>

ScriptView::ScriptView(QWidget* parent)
: QWidget{parent}
, _script{}
{
    auto layout = new QGridLayout;
    setLayout(layout);

    _labelName = new QLabel;
    _labelVersion = new QLabel;
    _labelAuthor = new QLabel;
    _labelDescription = new QLabel;

    layout->addWidget(new QLabel("Name"), 0, 0);
    layout->addWidget(new QLabel("Version"), 1, 0);
    layout->addWidget(new QLabel("Author"), 2, 0);
    layout->addWidget(new QLabel("Description"), 3, 0);

    layout->addWidget(_labelName, 0, 1);
    layout->addWidget(_labelVersion, 1, 1);
    layout->addWidget(_labelAuthor, 2, 1);
    layout->addWidget(_labelDescription, 3, 1);
}

void ScriptView::setScript(Script* script)
{
    _script = script;
    refresh();
}

void ScriptView::refresh()
{
    if (_script)
    {
        _labelName->setText(_script->name());
        _labelVersion->setText(_script->version());
        _labelAuthor->setText(_script->author());
        _labelDescription->setText(_script->description());
    }
    else
    {
        _labelName->setText("");
        _labelVersion->setText("");
        _labelAuthor->setText("");
        _labelDescription->setText("");
    }
}
