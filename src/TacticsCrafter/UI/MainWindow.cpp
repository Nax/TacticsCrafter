#include <QtWidgets>
#include <TacticsCrafter/UI/MainWindow.h>

MainWindow::MainWindow(QWidget* parent)
: QMainWindow{parent}
, _scriptManager{}
{
    setWindowTitle("TacticsCrafter");

    createActions();
    createMenus();
    createWidgets();
}

void MainWindow::importScript()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Import Script"), "", tr("Lua Script (*.lua)"));

    if (!filename.isEmpty())
    {
        _scriptManager.load(filename);
        _scriptManager.prerun();
        refreshScripts();
    }
}

void MainWindow::createActions()
{
    _actionImportScript = new QAction("Import Script...", this);
    connect(_actionImportScript, &QAction::triggered, this, &MainWindow::importScript);
}

void MainWindow::createMenus()
{
    auto m = menuBar();

    auto menuFile = m->addMenu(tr("&File"));
    menuFile->addAction(_actionImportScript);

    m->addMenu(tr("&Edit"));
    m->addMenu(tr("&About"));
}

void MainWindow::createWidgets()
{
    /* Script List */
    _widgetScriptList = new QListWidget();
    _widgetScriptList->setMinimumWidth(250);

    /* Script Viewer */
    auto scriptViewer = new QWidget;
    scriptViewer->setMinimumWidth(500);
    scriptViewer->setMinimumHeight(500);

    auto mainWidget = new QWidget;
    auto layout = new QHBoxLayout();
    layout->addWidget(_widgetScriptList);
    layout->addWidget(scriptViewer, 1);
    mainWidget->setLayout(layout);
    setCentralWidget(mainWidget);
}

void MainWindow::refreshScripts()
{
    _widgetScriptList->clear();
    for (std::size_t i = 0; i < _scriptManager.count(); ++i)
    {
        _widgetScriptList->addItem(_scriptManager.get(i).name());
    }
}
