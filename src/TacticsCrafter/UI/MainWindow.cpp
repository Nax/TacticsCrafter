#include <QtWidgets>
#include <TacticsCrafter/UI/MainWindow.h>
#include <TacticsCrafter/UI/TabBuild.h>
#include <TacticsCrafter/UI/TabScripts.h>

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
    auto tabWidget = new QTabWidget;
    tabWidget->addTab(new TabScripts{_scriptManager}, "Patches");
    tabWidget->addTab(new TabBuild, "Build");

    auto centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    auto centralLayout = new QVBoxLayout;
    centralWidget->setLayout(centralLayout);

    centralLayout->addWidget(tabWidget);
}
