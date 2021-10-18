#include <QtWidgets>
#include <TacticsCrafter/ScriptView.h>

ScriptView::ScriptView(LTC_Context* ctx, QWidget* parent)
: QWidget{parent}
, _ctx{ctx}
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

void ScriptView::setScript(LTC_Script script)
{
    _script = script;
    refresh();
}

void ScriptView::refresh()
{
    QWidget().setLayout(_optsContainer->layout());
    _optsContainer->setLayout(new QVBoxLayout);

    if (_script && ltcGetScriptString(_ctx, _script, LTC_SCRIPT_NAME))
    {
        _labelName->setText(ltcGetScriptString(_ctx, _script, LTC_SCRIPT_NAME));
        _labelVersion->setText(ltcGetScriptString(_ctx, _script, LTC_SCRIPT_VERSION));
        _labelAuthor->setText(ltcGetScriptString(_ctx, _script, LTC_SCRIPT_AUTHOR));
        _labelDescription->setText(ltcGetScriptString(_ctx, _script, LTC_SCRIPT_DESCRIPTION));

        _console->clear();
        const char* log = ltcGetScriptString(_ctx, _script, LTC_SCRIPT_LOG);
        if (log[0] == 0)
        {
            _consoleContainer->setVisible(false);
        }
        else
        {
            _consoleContainer->setVisible(true);
            _console->setText(log);
        }

        /* Options */
        int optCount = ltcGetOptionCount(_ctx, _script);
        for (int index = 0; index < optCount; ++index)
        {
            LTC_Option o = ltcGetOptionHandle(_ctx, _script, index);
            switch (ltcGetOptionType(_ctx, o))
            {
            case LTC_OPTION_BOOLEAN:
                auto checkbox = new QCheckBox(ltcGetOptionName(_ctx, o));
                checkbox->setCheckState(ltcGetOptionValueBoolean(_ctx, o) ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
                connect(checkbox, &QCheckBox::stateChanged, [=](int state)
                {
                    ltcSetOptionValueBoolean(_ctx, o, !!state);
                    ltcRunPipeline(_ctx);
                    emit changed();
                    refresh();
                });
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
