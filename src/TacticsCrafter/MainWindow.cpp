#include <QtWidgets>
#include <TacticsCrafter/MainWindow.h>
#include <TacticsCrafter/TabBuild.h>
#include <TacticsCrafter/TabScripts.h>

MainWindow::MainWindow(QWidget* parent)
: QMainWindow{parent}
{
    _ctx = ltcCreateContext((QCoreApplication::applicationDirPath() + "/data").toStdString().c_str());

    setWindowTitle("TacticsCrafter");

    createActions();
    createMenus();
    createWidgets();
}

MainWindow::~MainWindow()
{
    ltcDestroyContext(_ctx);
}

void MainWindow::importScript()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Import Script"), "", tr("Lua Script (*.lua)"));

    if (!filename.isEmpty())
    {
        ltcLoadScript(_ctx, filename.toStdString().c_str());
        ltcRunPipeline(_ctx);
        _tabScripts->refresh();
    }
}

void MainWindow::createActions()
{
    _actionImportScript = new QAction("Import Script...");
    connect(_actionImportScript, &QAction::triggered, this, &MainWindow::importScript);

    _actionQuit = new QAction("Quit");
    _actionQuit->setShortcut(QKeySequence("Ctrl+Q"));
    connect(_actionQuit, &QAction::triggered, [this](){ close(); });

    _actionAbout = new QAction(tr("About TacticsCrafter..."));
    connect(_actionAbout, &QAction::triggered, [this](){
        QMessageBox::about(this, "TacticsCrafter", "© 2021, Maxime Bacoux");
    });
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

    auto menuHelp = m->addMenu(tr("&Help"));
    menuHelp->addAction(_actionAbout);
}

void MainWindow::createWidgets()
{
    _tabScripts = new TabScripts{_ctx};

    auto tabWidget = new QTabWidget;
    tabWidget->addTab(_tabScripts, "Patches");
    tabWidget->addTab(new TabBuild{_ctx}, "Build");

    auto centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    auto centralLayout = new QVBoxLayout;
    centralWidget->setLayout(centralLayout);

    centralLayout->addWidget(tabWidget);
}
