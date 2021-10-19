#include <QtWidgets>
#include <libtactics/tactics.h>
#include <TacticsCrafter/TabScripts.h>
#include <TacticsCrafter/ScriptView.h>

TabScripts::TabScripts(LTC_Context** ctx, QWidget* parent)
: QWidget{parent}
, _ctx{ctx}
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

    auto btnUp = new QPushButton("Up");
    auto btnDown = new QPushButton("Down");
    auto btnRemove = new QPushButton("Remove");

    connect(btnUp, &QPushButton::pressed, this, &TabScripts::moveUp);
    connect(btnDown, &QPushButton::pressed, this, &TabScripts::moveDown);
    connect(btnRemove, &QPushButton::pressed, this, &TabScripts::remove);

    auto layoutControls = new QHBoxLayout;
    layoutControls->addWidget(btnUp);
    layoutControls->addWidget(btnDown);
    layoutControls->addWidget(btnRemove);

    auto listLayout = new QVBoxLayout;
    listLayout->addWidget(_list, 1);
    listLayout->addLayout(layoutControls);
    listLayout->addWidget(checkbox);

    /* Script Viewer */
    _view = new ScriptView{_ctx};
    _view->setMinimumWidth(500);
    _view->setMinimumHeight(500);

    auto layout = new QHBoxLayout();
    layout->addLayout(listLayout);
    layout->addWidget(_view, 1);
    setLayout(layout);
}

void TabScripts::refresh()
{
    int scriptCount = ltcGetScriptCount(*_ctx);

    _list->clear();

    for (int i = 0; i < scriptCount; ++i)
    {
        LTC_Script s = ltcGetScriptHandle(*_ctx, i);
        int core = ltcGetScriptInt(*_ctx, s, LTC_SCRIPT_CORE);
        int error = ltcGetScriptInt(*_ctx, s, LTC_SCRIPT_ERROR);

        if (!core || _showCore)
        {
            auto item = new QListWidgetItem;
            item->setText(ltcGetScriptString(*_ctx, s, LTC_SCRIPT_NAME));
            item->setForeground(error ? Qt::red : core ? Qt::darkBlue : Qt::black);
            item->setData(Qt::UserRole + 0, (int)s);
            _list->addItem(item);
        }
    }
}

void TabScripts::select(int index)
{
    auto item = _list->item(index);
    LTC_Script script = (LTC_Script)item->data(Qt::UserRole + 0).toInt();

    _view->setScript(script);
}

void TabScripts::moveUp()
{
    if (_list->selectedItems().empty())
        return;

    auto item = _list->selectedItems().first();
    LTC_Script s = (LTC_Script)item->data(Qt::UserRole + 0).toInt();

    ltcMoveScript(*_ctx, s, -1);
    refresh();
}

void TabScripts::moveDown()
{
    if (_list->selectedItems().empty())
        return;

    auto item = _list->selectedItems().first();
    LTC_Script s = (LTC_Script)item->data(Qt::UserRole + 0).toInt();

    ltcMoveScript(*_ctx, s, 1);
    refresh();
}

void TabScripts::remove()
{
    if (_list->selectedItems().empty())
        return;

    auto item = _list->selectedItems().first();
    LTC_Script s = (LTC_Script)item->data(Qt::UserRole + 0).toInt();

    ltcRemoveScript(*_ctx, s);
    refresh();
}
