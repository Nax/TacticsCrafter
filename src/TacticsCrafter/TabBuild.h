#ifndef TC_UI_TAB_BUILD_H
#define TC_UI_TAB_BUILD_H

#include <QWidget>
#include <QProgressBar>

struct LTC_Context;
class TabBuild : public QWidget
{
    Q_OBJECT

public:
    TabBuild(LTC_Context* ctx, QWidget* parent = nullptr);
    void report(int number);

private slots:
    void run();

private:
    LTC_Context* _ctx;

    QString         _fileIn;
    QString         _fileOut;
    QProgressBar*   _progress;
};

#endif /* TC_UI_TAB_BUILD_H */
