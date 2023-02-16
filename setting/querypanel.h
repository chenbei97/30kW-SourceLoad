#ifndef QUERYPANEL_H
#define QUERYPANEL_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QSplitter>
#include <QGroupBox>
#include <QIODevice>
#include <QTime>
#include "../messagebox/critical.h"
#include "../core/enum.h"

class QueryPanel : public QDialog
{
    Q_OBJECT
public:
    explicit QueryPanel(QWidget * parent = Q_NULLPTR);

    void setDevice(QIODevice*);
    QIODevice* device() const;
private:
    void initQueryPanel();
    QListWidget * mQueryPanel;
    QTextEdit * mDisplayPanel;
    QIODevice * mDevice;
    void setTextLineColor(const QColor&);
private slots:
    void terminalQuery(QListWidgetItem *);
    void display(const QByteArray&);
signals:
    void setTerminalCommand(const QByteArray&);
    void terminalQueryData(const QByteArray&);
};


#endif // QUERYPANEL_H
