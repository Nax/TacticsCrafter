#ifndef TC_UI_FILE_SELECTOR_H
#define TC_UI_FILE_SELECTOR_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class FileSelector : public QWidget
{
public:
    FileSelector(QWidget* parent = nullptr);

private:
    QLabel*         _label;
    QLineEdit*      _lineEdit;
    QPushButton*    _button;
};

#endif /* TC_UI_FILE_SELECTOR_H */
