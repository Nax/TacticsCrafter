#include <QtWidgets>
#include <TacticsCrafter/MainWindow.h>
#include <TacticsCrafter/TabBuild.h>
#include <TacticsCrafter/TabScripts.h>

namespace
{

const char* dataPath()
{
    static std::string p = (QCoreApplication::applicationDirPath() + "/data").toStdString();
    return p.c_str();
}

}

MainWindow::MainWindow(QWidget* parent)
: QMainWindow{parent}
{
    _ctx = ltcCreateContext(dataPath(), nullptr);

    setWindowTitle("TacticsCrafter");

    createActions();
    createMenus();
    createWidgets();
}

MainWindow::~MainWindow()
{
    ltcDestroyContext(_ctx);
}

void MainWindow::open()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Project"), "", tr("TacticsCrafter Project File (*.tcproj)"));

    if (!filename.isEmpty())
    {
        ltcDestroyContext(_ctx);
        _ctx = ltcCreateContext(dataPath(), filename.toStdString().c_str());
        _tabScripts->refresh();
        _projectFilePath = filename;
    }
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

void MainWindow::save()
{
    if (_projectFilePath.isEmpty())
    {
        saveAs();
        return;
    }

    ltcSaveContext(_ctx, _projectFilePath.toStdString().c_str());
}

void MainWindow::saveAs()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save as..."), "", tr("TacticsCrafter Project File (*.tcproj)"));

    if (!filename.isEmpty())
    {
        _projectFilePath = filename;
        save();
    }
}

void MainWindow::createActions()
{
    _actionOpen = new QAction("Open");
    _actionOpen->setShortcut(QKeySequence("Ctrl+O"));
    connect(_actionOpen, &QAction::triggered, this, &MainWindow::open);

    _actionSave = new QAction("Save");
    _actionSave->setShortcut(QKeySequence("Ctrl+S"));
    connect(_actionSave, &QAction::triggered, this, &MainWindow::save);

    _actionSaveAs = new QAction("Save As...");
    _actionSaveAs->setShortcut(QKeySequence("Ctrl+Shift+S"));
    connect(_actionSaveAs, &QAction::triggered, this, &MainWindow::saveAs);

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
    menuFile->addAction(_actionOpen);
    menuFile->addSeparator();
    menuFile->addAction(_actionSave);
    menuFile->addAction(_actionSaveAs);
    menuFile->addSeparator();
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
    _tabScripts = new TabScripts{&_ctx};

    auto tabWidget = new QTabWidget;
    tabWidget->addTab(_tabScripts, "Patches");
    tabWidget->addTab(new TabBuild{&_ctx}, "Build");

    auto centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    auto centralLayout = new QVBoxLayout;
    centralWidget->setLayout(centralLayout);

    centralLayout->addWidget(tabWidget);
}
