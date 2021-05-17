#include <QtWidgets>
#include <TacticsCrafter/UI/TabScripts.h>
#include <TacticsCrafter/UI/ScriptView.h>
#include <TacticsCrafter/Core/ScriptManager.h>

TabScripts::TabScripts(ScriptManager& scripts, QWidget* parent)
: QWidget{parent}
, _scripts{scripts}
{
    /* Script List */
    _list = new QListWidget();
    _list->setMinimumWidth(250);
    connect(_list, &QListWidget::itemSelectionChanged, [this]()
    {
        select(_list->currentRow());
    });

    /* Script Viewer */
    _view = new ScriptView;
    _view->setMinimumWidth(500);
    _view->setMinimumHeight(500);

    auto layout = new QHBoxLayout();
    layout->addWidget(_list);
    layout->addWidget(_view, 1);
    setLayout(layout);

    connect(&_scripts, &ScriptManager::update, this, &TabScripts::refresh);
}

void TabScripts::select(int index)
{
    _view->setScript(&_scripts.get(index));
}

void TabScripts::refresh()
{
    _list->clear();
    for (std::size_t i = 0; i < _scripts.count(); ++i)
    {
        _list->addItem(_scripts.get(i).name());
    }
}
