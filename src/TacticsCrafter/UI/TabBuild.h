#ifndef TC_UI_TAB_BUILD_H
#define TC_UI_TAB_BUILD_H

#include <QWidget>
#include <QProgressBar>

class ScriptManager;
class TabBuild : public QWidget
{
    Q_OBJECT

public:
    TabBuild(ScriptManager& scripts, QWidget* parent = nullptr);

private slots:
    void run();

private:
    ScriptManager&  _scripts;
    QString         _fileIn;
    QString         _fileOut;
    QProgressBar*   _progress;
};

#endif /* TC_UI_TAB_BUILD_H */
