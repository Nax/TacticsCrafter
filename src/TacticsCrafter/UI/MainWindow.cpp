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
        _scriptManager.run();
    }
}

void MainWindow::createActions()
{
    _actionImportScript = new QAction("Import Script...");
    connect(_actionImportScript, &QAction::triggered, this, &MainWindow::importScript);

    _actionQuit = new QAction("Quit");
    _actionQuit->setShortcut(QKeySequence("Ctrl+Q"));
    connect(_actionQuit, &QAction::triggered, [this](){ close(); });
}

void MainWindow::createMenus()
{
    auto m = menuBar();

    auto menuFile = m->addMenu(tr("&File"));
    menuFile->addAction(_actionImportScript);
    menuFile->addSeparator();
    menuFile->addAction(_actionQuit);

    auto menuEdit = m->addMenu(tr("&Edit"));
    menuEdit->addAction(tr("Copy"))->setShortcut(QKeySequence("Ctrl+C"));
    menuEdit->addAction(tr("Paste"))->setShortcut(QKeySequence("Ctrl+V"));
    menuEdit->addAction(tr("Cut"))->setShortcut(QKeySequence("Ctrl+X"));

    m->addMenu(tr("&About"));
}

void MainWindow::createWidgets()
{
    auto tabWidget = new QTabWidget;
    tabWidget->addTab(new TabScripts{_scriptManager}, "Patches");
    tabWidget->addTab(new TabBuild{_scriptManager}, "Build");

    auto centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    auto centralLayout = new QVBoxLayout;
    centralWidget->setLayout(centralLayout);

    centralLayout->addWidget(tabWidget);
}
