#ifndef DELEGATE_H
#define DELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QSpinBox>

class Delegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit Delegate(QObject *parent = 0);
    
    friend class MainWindow;

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) ;
    void setEditorData(QWidget *editor, const QModelIndex &index) ;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) ;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) ;
signals:
    
public slots:
    
};

#endif // DELEGATE_H
