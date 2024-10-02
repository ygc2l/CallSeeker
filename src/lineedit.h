#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QDragEnterEvent>
#include <QLineEdit>

class LineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit LineEdit(QWidget *parent = Q_NULLPTR);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    QStringList filepaths;
};


#endif // LINEEDIT_H
