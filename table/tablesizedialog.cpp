#include "tablesizedialog.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

TableSizeDialog::TableSizeDialog(QWidget * parent):QDialog(parent)
{
        mainLayout = new QGridLayout;

        QLabel * mRowLabel = new QLabel(tr("行: "));
        QLabel * mColLabel = new QLabel(tr("列: "));
        mRowEdit = new QLineEdit;
        mColEdit = new QLineEdit;
        QIntValidator *validator = new QIntValidator(0,10000); // 用指针才起作用
        mRowEdit->setValidator(validator);
        mColEdit->setValidator(validator);
        QLabel * mHeaderLabel = new QLabel(tr("表头: "));
        mHeaderEdit = new QLineEdit(tr("A,B,C,或不输入(默认1,2,3)"));

        mOkBtn = new QPushButton(tr("确定"));
        mCancelBtn = new QPushButton(tr("取消"));
        mRow = -1;
        mCol = -1;

        // 主布局的第1个GroupBox: 自定义表格的尺寸和表头
        QGroupBox * sizeBox = new QGroupBox(tr("自定义表格"));
        QGridLayout * sizeLay = new QGridLayout;
        sizeLay->addWidget(mRowLabel,0,0);
        sizeLay->addWidget(mRowEdit,0,1);
        sizeLay->addWidget(mColLabel,1,0);
        sizeLay->addWidget(mColEdit,1,1);
        sizeLay->addWidget(mHeaderLabel,2,0);
        sizeLay->addWidget(mHeaderEdit,2,1);
        sizeBox->setLayout(sizeLay);

        // 主布局的第2个GroupBox: 多行文本导入表格
        QGroupBox * importBox = new QGroupBox(tr("文本导入"));
        QVBoxLayout * importLay = new QVBoxLayout;
        QString importTxt = tr("1. 如果不启用文本导入,必须在自定义表格设置行列和表头,"
                               "表头必须以逗号隔开,个数必须和列数一致,例如'A,B,C,D',或者不输入默认'1,2,3,4...'表头设置.\n"
                               "2. 使用文本导入功能就不要再设置表格行列数和表头,自定义表格内的功能无效.\n"
                               "3. 粘贴的文本要求带有表头否则会解析错误,同时每列务必用空格隔开(space,tab)或者逗号(,)隔开,每行用换行符隔开!\n"
                               "4. 这里的文本导入功能实际就是表格窗口中导入Txt/Csv的功能,只是提供了一个编辑框的功能更方便!\n"
                               "5. 注意,使用文本导入不要粘贴大量文本(万行级),因为这里没有并行优化很可能卡死."
                               "如果需要导入大量文本,请使用表格窗口导入Txt/Csv的功能!\n");
        QPlainTextEdit * txtEdit = new QPlainTextEdit(importTxt);
        txtEdit->setReadOnly(true);
        QLabel * isImportLabel = new QLabel(tr("是否启用文本导入?"));
        QCheckBox * isImport = new QCheckBox;
        connect(isImport,static_cast<void (QCheckBox::*)(bool)>(&QCheckBox::clicked),this,[=](bool checked){
             mRowEdit->setEnabled(!checked); //这里设置的表格行列和表头无效,不允许编辑,直接在plaintext定义
             mColEdit->setEnabled(!checked);
             mHeaderEdit->setEnabled(!checked);
             txtEdit->setReadOnly(!checked);
             checked?txtEdit->clear():txtEdit->setPlainText(importTxt);
        });
        isImport->setChecked(false);
        QHBoxLayout * isImportLay = new QHBoxLayout;
        isImportLay->addStretch();
        isImportLay->addWidget(isImportLabel);
        isImportLay->addWidget(isImport);
        importLay->addWidget(txtEdit);
        importLay->addLayout(isImportLay);
        importBox->setLayout(importLay);

        connect(mOkBtn,&QPushButton::clicked,this,[=]{
            mHeader.clear(); // 实现清除掉,否则下次会累积
            if (!isImport->isChecked()) // 不启用文本导入
            {
                    QString text1 = mRowEdit->text().simplified();
                    QString text2 = mColEdit->text().simplified();
                    QString text3 = mHeaderEdit->text().simplified(); // 逗号隔开的
                    if (text1.isEmpty() || text2.isEmpty()) return;
                    mRow = text1.toUInt();// row(),col(),header()把信息带出去
                    mCol = text2.toUInt();
                    if (!text3.isEmpty()) mHeader = text3.split(',',QString::SkipEmptyParts); // 去掉空格
                    else for(int i = 1; i <=mCol; ++i) mHeader<<QString::number(i); // 如果没有输入,则默认1,2,3,...col
                    //qDebug()<<mHeader;//"A", "B", "C", "D"
                    if (mCol != mHeader.size()){
                        Critical * c= new Critical;
                        c->setText(tr("表头项数和列数不匹配,请重新输入!"));
                        c->setDialogFixedSize(900,280);
                        c->setTextWrapIndent(2,16);
                        c->exec();
                        delete c;
                        mHeaderEdit->clear();
                        return;
                    }
                    emit customTableNotImport();//不使用文本导入,tableview获取header(),col(),row()信息来reset
            }
            else { // 启用文本导入,粘贴的文本用空格或者逗号隔开均可
                    QStringList content = txtEdit->toPlainText().split('\n',QString::SkipEmptyParts);
                    emit customTableUseImport(content);//传给tableview调用importTableConcurrent实现
            }
            accept();
        });
        connect(mCancelBtn,&QPushButton::clicked,this,&QDialog::reject);
        QHBoxLayout * lay = new QHBoxLayout;
        lay->addStretch();
        lay->addWidget(mOkBtn);
        lay->addWidget(mCancelBtn);

        mainLayout->addWidget(sizeBox,0,0);
        mainLayout->addWidget(importBox,1,0);
        mainLayout->addLayout(lay,2,0);

        setLayout(mainLayout);

        setWindowIcon(QIcon(":/images/tableview.png"));
        setFont(QFont("Times New Roman",12));
        //setFixedSize(QSize(1050,650));
        resize(QSize(1050,650));
        setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);//添加最大最小化和关闭按钮
        setWindowFlag(Qt::Window); // 设置window属性就不会总是让界面在前或者在后
        Qt::WindowFlags flags = windowFlags();
        setWindowFlags(flags^Qt::WindowContextHelpButtonHint); // 去除帮助按钮
}

int TableSizeDialog::row() const
{
        return mRow;
}

int TableSizeDialog::col() const
{
        return mCol;
}

QStringList TableSizeDialog::header() const
{
    return mHeader;
}

void TableSizeDialog::setCurrentSize(int row, int col)
{
    mRowEdit->setText(QString::number(row));
    mColEdit->setText(QString::number(col));
}

