#include <QtWidgets>
#include <TacticsCrafter/Core/Script.h>
#include <TacticsCrafter/UI/ScriptView.h>

ScriptView::ScriptView(QWidget* parent)
: QWidget{parent}
, _script{}
{
    auto layoutMeta = new QGridLayout;

    _labelName = new QLabel;
    _labelVersion = new QLabel;
    _labelAuthor = new QLabel;
    _labelDescription = new QLabel;

    layoutMeta->addWidget(new QLabel("Name"), 0, 0);
    layoutMeta->addWidget(new QLabel("Version"), 1, 0);
    layoutMeta->addWidget(new QLabel("Author"), 2, 0);
    layoutMeta->addWidget(new QLabel("Description"), 3, 0);

    layoutMeta->addWidget(_labelName, 0, 1);
    layoutMeta->addWidget(_labelVersion, 1, 1);
    layoutMeta->addWidget(_labelAuthor, 2, 1);
    layoutMeta->addWidget(_labelDescription, 3, 1);

    layoutMeta->setRowStretch(4, 1);
    layoutMeta->setColumnStretch(1, 1);

    auto consoleLayout = new QVBoxLayout;
    consoleLayout->addWidget(new QLabel("Console"));
    _console = new QTextEdit;
    _console->setReadOnly(true);
    consoleLayout->addWidget(_console, 1);
    _consoleContainer = new QWidget;
    _consoleContainer->setLayout(consoleLayout);
    _consoleContainer->setVisible(false);

    _optsContainer = new QWidget;
    _optsContainer->setLayout(new QVBoxLayout);

    auto layout = new QVBoxLayout;
    layout->addLayout(layoutMeta);
    layout->addWidget(_optsContainer);
    layout->addStretch(1);
    layout->addWidget(_consoleContainer);
    setLayout(layout);
}

void ScriptView::setScript(Script* script)
{
    _script = script;
    refresh();
}

void ScriptView::refresh()
{
    QWidget().setLayout(_optsContainer->layout());
    _optsContainer->setLayout(new QVBoxLayout);

    if (_script)
    {
        const auto& m = _script->meta();

        _labelName->setText(m.name);
        _labelVersion->setText(m.version);
        _labelAuthor->setText(m.author);
        _labelDescription->setText(m.description);

        _console->clear();
        if (_script->log().empty())
        {
            _consoleContainer->setVisible(false);
        }
        else
        {
            _consoleContainer->setVisible(true);
            for (const auto& l : _script->log())
                _console->append(l);
        }

        /* Options */
        for (auto& o : _script->opts())
        {
            switch (o.type)
            {
            case Script::Option::Type::Bool:
                auto checkbox = new QCheckBox(o.text);
                checkbox->setCheckState(o.b ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
                connect(checkbox, &QCheckBox::stateChanged, [&](int state){ o.b = !!state; emit changed(); });
                _optsContainer->layout()->addWidget(checkbox);
                break;
            }
        }

    }
    else
    {
        _labelName->setText("");
        _labelVersion->setText("");
        _labelAuthor->setText("");
        _labelDescription->setText("");

        _consoleContainer->setVisible(false);
    }
}
