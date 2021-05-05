#ifndef TC_MAIN_WINDOW_H
#define TC_MAIN_WINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <TacticsCrafter/Core/ScriptManager.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private slots:
    void importScript();

private:
    void createActions();
    void createMenus();
    void createWidgets();

    void refreshScripts();

    ScriptManager   _scriptManager;

    QAction* _actionImportScript;

    QListWidget*    _widgetScriptList;
};

#endif /* TC_MAIN_WINDOW_H */
