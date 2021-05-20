#ifndef TC_UI_TAB_SCRIPTS_H
#define TC_UI_TAB_SCRIPTS_H

#include <QWidget>
#include <QListWidget>

class ScriptManager;
class ScriptView;
class TabScripts : public QWidget
{
    Q_OBJECT

public:
    TabScripts(ScriptManager& scripts, QWidget* parent = nullptr);

private slots:
    void select(int index);
    void moveUp();
    void moveDown();
    void remove();
    void refresh();

private:
    ScriptManager&  _scripts;
    QListWidget*    _list;
    ScriptView*     _view;
    bool            _showCore;
};


#endif /* TC_UI_TAB_SCRIPTS_H */
