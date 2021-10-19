#ifndef TC_UI_SCRIPT_VIEW_H
#define TC_UI_SCRIPT_VIEW_H

#include <libtactics/tactics.h>
#include <QWidget>
#include <QLabel>
#include <QTextEdit>

class ScriptView : public QWidget
{
    Q_OBJECT

public:
    ScriptView(LTC_Context** ctx, QWidget* parent = nullptr);

    void setScript(LTC_Script script);
    void refresh();

signals:
    void changed();

private:
    LTC_Context** _ctx;
    LTC_Script _script;

    QLabel* _labelName;
    QLabel* _labelVersion;
    QLabel* _labelAuthor;
    QLabel* _labelDescription;

    QWidget*    _consoleContainer;
    QWidget*    _optsContainer;
    QTextEdit*  _console;
};

#endif /* TC_UI_SCRIPT_VIEW_H */
