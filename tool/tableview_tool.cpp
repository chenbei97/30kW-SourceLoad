#include "tableview_tool.h"

TableViewWidget::TableViewWidget(QWidget *parent): QMainWindow(parent),
    mTableView(new QTableView),mTable(new Table(mTableView,12,10)),mTableSizeDlg(new TableSizeDialog),
    mCellLoc(new QLabel(tr("位置: 未知"))), mCellContent(new QLabel(tr("文本: 未知"))),mToolTip(new QLabel(tr("工具提示: 未知"))),
    ActAppendRow(new QAction(tr("添加行"))),ActAppendCol(new QAction(tr("添加列"))),
    ActInsertRow(new QAction(tr("插入行"))),ActInsertCol(new QAction(tr("插入列"))),
    ActRemoveRow(new QAction(tr("删除行"))),ActRemoveCol(new QAction(tr("删除列"))),
    ActSetBold(new QAction(tr("加粗"))),ActSetItalic(new QAction(tr("斜体"))),ActClearTable(new QAction(tr("清空"))),ActSetColor(new QAction(tr("画笔"))),
    ActSetAlignLeft(new QAction(tr("居左"))),ActSetAlignCenter(new QAction(tr("居中"))),ActSetAlignRight(new QAction(tr("居右"))),
    ActImportTxt(new QAction(tr("导入Txt"))),ActExportTxt(new QAction(tr("导出Txt"))),
    ActImportCsv(new QAction(tr("导入Csv"))),ActExportCsv(new QAction(tr("导出Csv"))),
    ActConfigTable(new QAction(tr("设置")))
{
        setMouseTracking(true);
        initUI();
        initConnections();
}

void TableViewWidget::initUI()
{
    QStatusBar * bar = new QStatusBar(this);
    mCellLoc->setMinimumWidth(100);
    mCellContent->setMinimumWidth(200);
    mToolTip->setMinimumWidth(100);
    bar->addWidget(mCellLoc);
    bar->addWidget(mCellContent);
    bar->addWidget(mToolTip);
    setStatusBar(bar);

    ActSetBold->setCheckable(true); // 这里为了让单元格对齐/加粗/斜体的状态和当前动作保持一致
    ActSetItalic->setCheckable(true);
    ActSetAlignLeft->setCheckable(true);
    ActSetAlignCenter->setCheckable(true);
    ActSetAlignRight->setCheckable(true);
    QActionGroup *alignGroup = new QActionGroup(this);
    alignGroup->addAction(ActSetAlignLeft);
    alignGroup->addAction(ActSetAlignCenter);
    alignGroup->addAction(ActSetAlignRight);
    ActSetAlignCenter->setChecked(true); // 默认居中对齐

    QToolBar * topbar = new QToolBar;
    topbar->setMovable(true);
    topbar->setFloatable(true);
    topbar->addAction(ActAppendRow);
    topbar->addAction(ActAppendCol);
    topbar->addAction(ActInsertRow);
    topbar->addAction(ActInsertCol);
    topbar->addAction(ActRemoveRow);
    topbar->addAction(ActRemoveCol);
    topbar->addSeparator();
    topbar->addAction(ActSetAlignLeft);
    topbar->addAction(ActSetAlignCenter);
    topbar->addAction(ActSetAlignRight);
    topbar->addAction(ActSetBold);
    topbar->addAction(ActSetItalic);
    addToolBar(Qt::TopToolBarArea,topbar);

    QToolBar * bottombar = new QToolBar;
    bottombar->setMovable(true);
    bottombar->setFloatable(true);
    bottombar->addAction(ActClearTable);
    bottombar->addAction(ActSetColor);
    bottombar->addSeparator();
    bottombar->addAction(ActImportTxt);
    bottombar->addAction(ActExportTxt);
    bottombar->addAction(ActImportCsv);
    bottombar->addAction(ActExportCsv);
    bottombar->addSeparator();
    bottombar->addAction(ActConfigTable);
    addToolBar(Qt::BottomToolBarArea,bottombar);

    setCentralWidget(mTableView);
    setWindowIcon(QIcon(":/images/tableview.png"));
    setFont(QFont("Times New Roman",12));
    resize(1350,700);
}

void TableViewWidget::initConnections()
{
     initActConnections();

     connect(mTable,static_cast<void (Table::*)(const QModelIndex&)>(&Table::cellCurrentIndex),this,
             [=](const QModelIndex&idx){ // 来自Table选择模型提供的有效单元格位置
             QString text = tr("位置: ( %1, %2 )").arg(idx.row()+1).arg(idx.column()+1);
             mCellLoc->setText(text);
             updateCellTextFont(idx);//使用tab/→也能显示动作的checked状态
     });

     connect(mTable,static_cast<void (Table::*)(const QString&)>(&Table::cellCurrentContent),this,
             [=](const QString&content){ // 来自Table选择模型提供的有效单元格位置
             QString text = tr("文本: %1").arg(content);
             mCellContent->setText(text);
     });

     connect(mTableView,static_cast<void (QTableView::*)(const QModelIndex&)>(&QTableView::clicked),this, // doubleClicked,pressed
             [=](const QModelIndex& index){updateCellTextFont(index);}); // 让动作的checked状态和单元格的状态保持一致


     connect(mTableSizeDlg,&TableSizeDialog::customTableNotImport,this,[=]{
         mTable->reset(mTableSizeDlg->row(),mTableSizeDlg->col());//重置行列数在前
         mTable->setHeader(mTableSizeDlg->header()); // 重置表头在后,否则reset会清除掉表头
     });

     connect(mTableSizeDlg,static_cast<void (TableSizeDialog::*)(QStringList&)>(&TableSizeDialog::customTableUseImport),
             this,[=](QStringList&content){ mTable->importTableConcurrent(content);
     });

}

void TableViewWidget::initActConnections()
{
    connect(ActAppendRow,&QAction::triggered,this,[=]{mTable->appendRow();});
    connect(ActAppendCol,&QAction::triggered,mTable,&Table::appendCol);
    connect(ActInsertRow,&QAction::triggered,this,[=]{mTable->insertRow();});
    connect(ActInsertCol,&QAction::triggered,mTable,&Table::insertCol);
    connect(ActRemoveRow,&QAction::triggered,mTable,&Table::removeRow);
    connect(ActRemoveCol,&QAction::triggered,mTable,&Table::removeCol);

    //信号函数参数必须≥槽函数参数,所以无参信号不能直接连接有参槽函数,这里用匿名函数解决
    connect(ActSetAlignLeft,&QAction::triggered,this,[=]{ // 没有再次点击对齐会取消对齐的行为
            mCellFont.align = Table::AlignType::Left;// 左对齐再次点击是左对齐
            mTable->setAlign(mCellFont.align);
    });
    connect(ActSetAlignCenter,&QAction::triggered,this,[=]{
            mCellFont.align = Table::AlignType::Center; // 居中对齐再次点击还是居中对齐
            mTable->setAlign(mCellFont.align);
    });
    connect(ActSetAlignRight,&QAction::triggered,this,[=]{
            mCellFont.align = Table::AlignType::Right; // 右对齐再次点击是右对齐
            mTable->setAlign(mCellFont.align);
    });

    connect(ActSetBold,&QAction::triggered,this,[=]{ // 加粗和斜体有取消效果
            mCellFont.bold=!mCellFont.bold;
            mTable->setBold(mCellFont.bold);// 再次点击会取消加粗
    });
    connect(ActSetItalic,&QAction::triggered,this,[=]{
            mCellFont.italic=!mCellFont.italic;
            mTable->setItalic(mCellFont.italic);// 再次点击会取消斜体
    });

    connect(ActClearTable,&QAction::triggered,mTable,[=]{mTable->clearConcurrent(true);});

    connect(ActConfigTable,&QAction::triggered,this,[=]{
        mTableSizeDlg->setCurrentSize(mTable->row(),mTable->col());//size对话框的文本显示和当前表格行列数一致
        mTableSizeDlg->exec();
    });

    connect(ActImportTxt,&QAction::triggered,mTable,&Table::importTxtConcurrent);//Concurrent
    connect(ActExportTxt,&QAction::triggered,mTable,&Table::exportTxtConcurrent);
    connect(ActImportCsv,&QAction::triggered,mTable,&Table::importCsvConcurrent);
    connect(ActExportCsv,&QAction::triggered,mTable,&Table::exportCsvConcurrent);

    connect(ActSetColor,&QAction::triggered,this,[=]{
            QPalette palette;
            palette.setColor(QPalette::Text,mCellFont.mForeColor);
            palette.setColor(QPalette::Background,mCellFont.mBackColor);
            TableColorDialog * dlg = new TableColorDialog(palette); // 传递初始颜色/或者上次设置过的颜色
            int ret = dlg->exec();
            if (ret == QDialog::Accepted)
            {
                  mCellFont.mForeColor = dlg->foregroundColor();//更新选中的新颜色
                  mCellFont.mBackColor = dlg->backgroundColor();
                  // 同时把颜色传递给mTable,内部去更新选中单元格的颜色
                  mTable->setColor(mCellFont.mForeColor,mCellFont.mBackColor);
            }
            delete dlg;
    });

    // 多个动作的同一信号执行相似的槽函数,可以考虑信号映射器,信号不能带参数
    QList<QAction*> actList;
    actList <<ActAppendRow <<ActAppendCol << ActInsertRow << ActInsertCol
                <<ActRemoveRow <<ActRemoveCol << ActSetAlignLeft << ActSetAlignCenter
               << ActSetAlignRight << ActSetBold << ActSetItalic << ActClearTable << ActSetColor
               << ActImportTxt<< ActExportTxt << ActImportCsv<<ActExportCsv << ActConfigTable ;
    QAction * act;
    QSignalMapper * signalMapper = new QSignalMapper(this);
    foreach(act,actList) {
            connect(act, SIGNAL(hovered()), signalMapper, SLOT(map()));
            signalMapper->setMapping(act,act->text()); // 绑定的文本映射关系
    }
    connect(signalMapper, static_cast<void (QSignalMapper::*)(const QString&)>(&QSignalMapper::mapped),
            this, [=](const QString& text){ QString t = tr("工具提示: %1").arg(text); mToolTip->setText(t);
    });
}


void TableViewWidget::updateCellTextFont(const QModelIndex &index)
{
    QStandardItem * item = mTable->takeItem(index); // 拿到这个单元格的指针
    ActSetBold->setChecked(item->font().bold()); // 让动作的状态和当前结果一致
    ActSetItalic->setChecked(item->font().italic());
    switch (item->textAlignment()) { // 动作使用分组,可以不手动实现互斥效果
//        case Qt::AlignLeft | Qt::AlignVCenter: ActSetAlignLeft->setChecked(true);
//            ActSetAlignCenter->setChecked(false);ActSetAlignRight->setChecked(false);break;
//        case Qt::AlignCenter:ActSetAlignCenter->setChecked(true);
//            ActSetAlignLeft->setChecked(false);ActSetAlignRight->setChecked(false);break;
//        case Qt::AlignRight | Qt::AlignVCenter:ActSetAlignRight->setChecked(true);
//            ActSetAlignLeft->setChecked(false);ActSetAlignCenter->setChecked(false);break;
//        default: break;
        case Qt::AlignLeft | Qt::AlignVCenter: ActSetAlignLeft->setChecked(true);break;
        case Qt::AlignCenter:ActSetAlignCenter->setChecked(true);break;
        case Qt::AlignRight | Qt::AlignVCenter:ActSetAlignRight->setChecked(true);break;
        default: break;
    }
}
