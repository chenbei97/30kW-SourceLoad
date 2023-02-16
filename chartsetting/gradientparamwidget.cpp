#include "gradientparamwidget.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

GradientParamWidget ::GradientParamWidget (QGradient::Type type,QWidget *parent) : QWidget(parent),mGradientType(type)
{
        // 渐变填充高级参数设置窗口，父窗口是背景区域工具栏下的-渐变填充工具组，再上级是背景区域设置的工具栏

        setFont(QFont("Times New Roman",12));
        setMinimumSize(600,400);
        setWindowIcon(QIcon(":/images/toolbox_chart.png"));

        mCriticalDlg = new Critical;
        mCriticalDlgIsCalled = false;

        // 1. 文本区域
        QLabel * deslabel = new QLabel(tr("渐变参数含义说明"));
        mGradientDescription = new QTextEdit;
        mGradientDescription->setReadOnly(true);
        mGradientDescription->setMinimumHeight(130); // 130对应4行文字差不多
        //mGradientDescription->setMaximumHeight(400);
        QVBoxLayout * deslay = new QVBoxLayout;
        deslay->addWidget(deslabel);
        deslay->addWidget(mGradientDescription);
        setGradientDescription(tr("1.渐变参数使用<step,color>来描述.其中step表示步数,范围[0.0,1.0];color表示颜色.\n"
                                  "2.允许通过按钮选择默认提供的11个颜色,也可以文本输入自定义间隔的颜色.\n"
                                  "3.文本必须用分号分隔开,键和值用逗号隔开,结尾可以没有分号.\n"
                                  "4.step至少有2个,只设置1个等同于颜色填充而非渐变填充.\n"
                                  "5.step允许不设置0.0和1.0,例如只设置0.2和0.6的颜色也是可以的.\n"
                                  "6.step允许使用0,1代替0.0和1.0\n"
                                  "7.color支持#或rgb(a)表示,注意rgb(a)值必须是整数且在[0,255]之间.\n"
                                  "8.文本示例: '0.0,#90EE90;0.5,rgb(255,0,0);1,rgba(0,0,255,200)'."));
        // 2. 中间区域
        mGradientLayout = new QVBoxLayout; // 这里的布局不同渐变类型不同,子类来实现
        // 2.2 按钮设置区域
        mParamSelector = new GradientParamSelectorWidget ;
        QGroupBox * group1 = new QGroupBox(tr("按钮选择"));
        QHBoxLayout * lay1 = new QHBoxLayout;
        lay1->addWidget(mParamSelector);
        group1 ->setLayout(lay1);
        // 2.3 文本输入区域
        QLineEdit * lineEdit = new QLineEdit("0.0,#90EE90;0.5,rgb(255,0,0);1.0,rgba(0,0,255,200)");
        lineEdit->setMinimumHeight(50);
        lineEdit->setStyleSheet("border:2px solid black;border-radius:10px;padding: 0 8px;"
                                "background:#FFA07A;selection-background-color:red;"
                                "font-size:24px;font-family:Times New Roman;");
        QGroupBox * group2 = new QGroupBox(tr("文本输入"));
        QHBoxLayout * lay2 = new QHBoxLayout;
        lay2->addWidget(lineEdit);
        group2->setLayout(lay2);
        group2->setEnabled(false);
        // 2.1 设置方式区域
        mSelectColorBtn = new QRadioButton(tr("按钮选择"));
        mSelectTextBtn = new QRadioButton(tr("文本输入"));
        connect(mSelectColorBtn,static_cast<void (QRadioButton::*)(bool)>(&QRadioButton::clicked),
                this,[=](bool checked){
                group1->setEnabled(checked);
                group2->setEnabled(!checked);
                if (checked) mParamSelector->setDefaultStyle();
        });
        connect(mSelectTextBtn,static_cast<void (QRadioButton::*)(bool)>(&QRadioButton::clicked),
                this,[=](bool checked){
                group1->setEnabled(!checked);
                group2->setEnabled(checked);
                if (checked) mParamSelector->setGrayStyle(); // 禁用按钮选择时按钮都变成灰色
        });
        mSelectColorBtn->setChecked(true);
        QGroupBox * group0 = new QGroupBox(tr("设置方式"));
        QHBoxLayout * lay0 = new QHBoxLayout;
        lay0->addWidget(mSelectColorBtn);lay0->addWidget(mSelectTextBtn);
        group0->setLayout(lay0);

        addGradientWidget(group0);
        addGradientWidget(group1);
        addGradientWidget(group2);

        // 3. 底部区域
        QHBoxLayout * btnlay = new QHBoxLayout;
        mOkBtn = new QPushButton(tr("设置完成"));
        connect(mOkBtn,&QPushButton::clicked,this,[=]{ // 把按钮选择的或者文本输入的颜色对发出去
            if (mSelectColorBtn->isChecked()) emit gradientParams(mParamSelector->selectedColors(),mGradientType);
           else{
                QVector<QPair<qreal,QColor>> list = inputedColors(lineEdit->text());
                if (!list.isEmpty()) emit gradientParams(list,mGradientType);
            }
            close(); // 子类根据需要额外绑定新的槽函数
        });
        btnlay->addStretch();
        btnlay->addWidget(mOkBtn);

        QVBoxLayout * lay = new QVBoxLayout;
        lay->addLayout(deslay);
        lay->addLayout(mGradientLayout);
        lay->addLayout(btnlay);
        setLayout(lay);
}

QVector<QPair<qreal,QColor>>GradientParamWidget::defaultColors() const
{
    return mParamSelector->defaultColors();
}

void GradientParamWidget ::setGradientDescription(const QString & des)
{
    mGradientDescription->setText(des);
}

void GradientParamWidget ::addGradientDescription(const QString & des)
{
    mGradientDescription->append(des);
}

void GradientParamWidget ::addGradientLayout(QLayout*lay)
{
    mGradientLayout->addLayout(lay); // 提供4个方法给外部使用,添加布局或者小部件均可
}

void GradientParamWidget ::addGradientWidget(QWidget * w)
{
    mGradientLayout->addWidget(w);
}

void GradientParamWidget ::insertGradientLayout(int index,QLayout*lay)
{
    mGradientLayout->insertLayout(index,lay);
}

void GradientParamWidget ::insertGradientWidget(int index,QWidget * w)
{
    mGradientLayout->insertWidget(index,w);
}

QVector<QPair<qreal,QColor>> GradientParamWidget::inputedColors(const QString&text)
{
    QVector<QPair<qreal,QColor>> list;
    if (text.isEmpty()) return list;

    QStringList stepColors = text.simplified().split(";",QString::SkipEmptyParts);
    // 检查每个stepColor对是否符合格式
    foreach(const QString& stepcolor, stepColors){
        if (!checkStepColor(stepcolor))
        {
            if (!mCriticalDlgIsCalled){ // 因为其他类型的错误没有调用对话框,例如输入了rbg(0,0,0,0)
                //
                mCriticalDlg->setText(tr("输入文本格式错误!"));
                mCriticalDlg->setTextWrapIndent(3,18);
                mCriticalDlg->exec();
            }
            return list;
        }
    }

    // 到这里说明符合格式,再遍历一遍取出值
    foreach(const QString& stepcolor, stepColors) {
        list << takeStepColor(stepcolor);
    }
    return list;
}

bool GradientParamWidget::checkStepColor(const QString& stepcolor)
{// 0.0,#90EE90 0.5,rgb(255,0,0) 1.0,rgba(0,0,255,200)
    // 按照逗号的个数来判断比较好
    bool ret = false; // 不是以下情况都为false
    if (stepcolor.contains(',')) // 有逗号
    {
        int count = stepcolor.count(','); // 逗号个数
        switch (count) {
            case 1:
                if (stepcolor.contains('#')) // 有#字符
                {
                    QStringList r = stepcolor.split(',',QString::SkipEmptyParts);
                    QString step = r.at(0);
                    QString color = r.at(1);
                    ret = checkColor(color) && checkStep(step);
                    //qDebug()<<"# ret = "<<ret;
                }
                break;
            case 3:
                if (stepcolor.contains(",rgb(") && stepcolor.endsWith(")"))
                {
                    int pos = stepcolor.indexOf(',');
                    QString step = stepcolor.left(pos);
                    QList<int> rgb = takeRgb(stepcolor,3);
                    ret = checkStep(step) && !rgb.isEmpty(); // rgb不为空说明颜色一定有效
                    //qDebug()<<"rgb ret = "<<ret<<" rgb = "<<rgb;
                }
                break;
            case 4:
                if (stepcolor.contains(",rgba(")&&stepcolor.endsWith(")")) //,rgb(255,0,0,1)有4个逗号,但是因为不包含,rgba所以返回false
                {
                    int pos = stepcolor.indexOf(',');
                    QString step = stepcolor.left(pos);
                    QList<int> rgba = takeRgb(stepcolor,4);
                    ret = checkStep(step) && !rgba.isEmpty();// rgba不为空说明颜色一定有效
                    //qDebug()<<"rgba ret = "<<ret <<"  rgba = "<<rgba;
                }
                break;
            default: break; // 其他情况均为false
        }
    }
    return ret;
}

QPair<qreal,QColor> GradientParamWidget::takeStepColor(const QString & stepcolor)
{ // 这个函数用在已经检查之后
        QPair<qreal,QColor> pair;
        int count = stepcolor.count(','); // 逗号个数
        int pos = 0; // 注意这4个临时变量必须先定义不要在分支内定义
        QStringList r; // cannot jump from switch statement to this case label
        QList<int> rgb;
        QList<int> rgba;

        switch (count)
        {
            case 1: // 说明是#字符
                    r = stepcolor.split(',',QString::SkipEmptyParts);
                    pair.first = r.at(0).toDouble();
                    pair.second  = QColor(r.at(1));
                    break;
            case 3: // 说明是rgb值
                    pos = stepcolor.indexOf(',');
                    pair.first= stepcolor.left(pos).toDouble();
                    rgb = takeRgb(stepcolor,3);
                    pair.second = QColor(rgb.at(0),rgb.at(1),rgb.at(2));
                    break;
            case 4: // 说明是rgba值
                    pos = stepcolor.indexOf(',');
                    pair.first = stepcolor.left(pos).toDouble();
                    rgba = takeRgb(stepcolor,4);
                    pair.second = QColor(rgba.at(0),rgba.at(1),rgba.at(2),rgba.at(3));
                    break;
            default: break;
        }
        return pair;
}

bool GradientParamWidget::checkStep(const QString &step)
{
    bool ok;
    double stepval = step.toDouble(&ok);
    bool r = ok &&(stepval<=1.0) &&(stepval>=0.0);
    if (!r) {
        mCriticalDlg->setText(tr("输入的步值[%1]无效!").arg(step));
        mCriticalDlg->setTextWrapIndent(3,18);
        mCriticalDlg->exec();
        mCriticalDlgIsCalled = true;
    }
    return r; // 数字有效且在[0.0,1.0]内
}

bool GradientParamWidget::checkColor(const QString &color)
{
    bool ret = QColor(color).isValid();
    if (!ret) {
        mCriticalDlg->setText(tr("输入的颜色%1无效!").arg(color));
        mCriticalDlg->setTextWrapIndent(3,18);
        mCriticalDlg->exec();
        mCriticalDlgIsCalled = true;
    }
     return ret; // 颜色是否有效
}

QList<int> GradientParamWidget::takeRgb(const QString &text,int count)
{ // 用于取出()内的rgb值
    QList<int> list;

    int pos1 = text.indexOf('(');
    int pos2 = text.indexOf(')');
    // pos1+1才是第1个要取的字符,mid是从pos开始取的
    // 取的字符个数应该是2个位置之差-1
    QString s = text.mid(pos1+1,pos2-pos1-1);

    if (s.count(',') == count-1)// 逗号只能是2个或3个
    {
        QStringList rgb = s.split(',',QString::SkipEmptyParts);
        if (rgb.size() == count)  // 只有3个值rgb或4个值 rgba
        {
            foreach(const QString& c, rgb)
            {
                if (!checkRgb(c)) {
                    if (count == 3){
                        mCriticalDlg->setText(tr("输入的颜色[rgb(%1,%2,%3)]无效!")
                                              .arg(rgb[0]).arg(rgb[1]).arg(rgb[2]));
                    }
                    if (count == 4){
                        mCriticalDlg->setText(tr("输入的颜色[rgba(%1,%2,%3,%4)]无效!")
                                              .arg(rgb[0]).arg(rgb[1]).arg(rgb[2]).arg(rgb[3]));
                    }
                    mCriticalDlg->setTextWrapIndent(3,18);
                    mCriticalDlg->exec();
                    mCriticalDlgIsCalled = true;
                    return QList<int>(); // rgb值数字不正确直接返回空链表
                }
                list << c.toInt(); // 3个条件全满足,颜色一定有效
            }
        }
    }
    return list;
}

bool GradientParamWidget::checkRgb(const QString &c)
{
    bool ok;
    int cval = c.toInt(&ok);
    return ok && (cval<=255) &&(cval>=0); // 数字有效且在[0,255]内
}
