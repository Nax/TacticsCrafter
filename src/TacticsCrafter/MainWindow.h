#ifndef TC_MAIN_WINDOW_H
#define TC_MAIN_WINDOW_H

#include <QMainWindow>
#include <libtactics/tactics.h>

class TabScripts;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow();

private slots:
    void importScript();

private:
    void createActions();
    void createMenus();
    void createWidgets();

    LTC_Context* _ctx;

    TabScripts* _tabScripts;

    QAction*    _actionImportScript;
    QAction*    _actionQuit;
    QAction*    _actionAbout;
};

#endif /* TC_MAIN_WINDOW_H */
