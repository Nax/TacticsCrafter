#ifndef TC_UI_TAB_SCRIPTS_H
#define TC_UI_TAB_SCRIPTS_H

#include <QWidget>
#include <QListWidget>

struct LTC_Context;
class ScriptView;
class TabScripts : public QWidget
{
    Q_OBJECT

public:
    TabScripts(LTC_Context** ctx, QWidget* parent = nullptr);
    void refresh();

private slots:
    void select(int index);
    void moveUp();
    void moveDown();
    void remove();

private:
    LTC_Context**   _ctx;

    QListWidget*    _list;
    ScriptView*     _view;
    bool            _showCore;
};


#endif /* TC_UI_TAB_SCRIPTS_H */
