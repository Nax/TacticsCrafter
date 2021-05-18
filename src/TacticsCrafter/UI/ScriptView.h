#ifndef TC_UI_SCRIPT_VIEW_H
#define TC_UI_SCRIPT_VIEW_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>

class Script;
class ScriptView : public QWidget
{
    Q_OBJECT

public:
    ScriptView(QWidget* parent = nullptr);

    void setScript(Script* script);
    void refresh();

signals:
    void changed();

private:
    Script* _script;

    QLabel* _labelName;
    QLabel* _labelVersion;
    QLabel* _labelAuthor;
    QLabel* _labelDescription;

    QWidget*    _consoleContainer;
    QWidget*    _optsContainer;
    QTextEdit*  _console;
};

#endif /* TC_UI_SCRIPT_VIEW_H */
