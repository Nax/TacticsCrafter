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

    void setLabel(const QString label);
    void setDialogOptions(const QString& title, const QString& filter, bool allowNew = false);

private slots:
    void selectFile();

private:
    QLabel*         _label;
    QLineEdit*      _lineEdit;
    QPushButton*    _button;

    QString         _dialogTitle;
    QString         _dialogFilter;
    bool            _dialogAllowNewFile;
};

#endif /* TC_UI_FILE_SELECTOR_H */
