#include <QtWidgets>
#include <TacticsCrafter/UI/TabScripts.h>
#include <TacticsCrafter/UI/ScriptView.h>
#include <TacticsCrafter/Core/ScriptManager.h>

TabScripts::TabScripts(ScriptManager& scripts, QWidget* parent)
: QWidget{parent}
, _scripts{scripts}
, _showCore{}
{
    /* Script List */
    _list = new QListWidget();
    _list->setMinimumWidth(250);
    connect(_list, &QListWidget::itemSelectionChanged, [this]()
    {
        select(_list->currentRow());
    });

    auto checkbox = new QCheckBox("Show Core Scripts");
    connect(checkbox, &QCheckBox::stateChanged, [this](int state){ _showCore = !!state; refresh(); });

    auto listLayout = new QVBoxLayout;
    listLayout->addWidget(_list, 1);
    listLayout->addWidget(checkbox);

    /* Script Viewer */
    _view = new ScriptView;
    _view->setMinimumWidth(500);
    _view->setMinimumHeight(500);

    auto layout = new QHBoxLayout();
    layout->addLayout(listLayout);
    layout->addWidget(_view, 1);
    setLayout(layout);

    connect(&_scripts, &ScriptManager::update, this, &TabScripts::refresh);
}

void TabScripts::select(int index)
{
    auto item = _list->item(index);
    std::size_t i = (std::size_t)item->data(Qt::UserRole + 0).toInt();
    _view->setScript(&_scripts.get(i));
}

void TabScripts::refresh()
{
    _list->clear();
    for (std::size_t i = 0; i < _scripts.count(); ++i)
    {
        auto const& s = _scripts.get(i);
        if (!s.core() || _showCore)
        {
            auto item = new QListWidgetItem;
            item->setText(s.meta().name);
            item->setData(Qt::UserRole + 0, (int)i);
            _list->addItem(item);
        }
    }
}
