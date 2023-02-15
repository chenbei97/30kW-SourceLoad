QT       += core gui
QT       += serialport
QT       += network
QT       += xml
QT       += concurrent
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += Qt_NO_CAST_FROM_ASCII

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TRANSLATIONS = cn.ts \
                                  en.ts

SOURCES += \
    axissetting/axisbarcategory.cpp \
    axissetting/axisbox.cpp \
    axissetting/axisgrid.cpp \
    axissetting/axisinfo.cpp \
    axissetting/axislabel.cpp \
    axissetting/axisline.cpp \
    axissetting/axislog.cpp \
    axissetting/axissetting.cpp \
    axissetting/axisshade.cpp \
    axissetting/axistime.cpp \
    axissetting/axistitle.cpp \
    axissetting/axisvalue.cpp \
    chart/barchart.cpp \
    chart/chart.cpp \
    chart/chartshowlegend.cpp \
    chart/chartshowtip.cpp \
    chart/linechart.cpp \
    chart/chartdatagenerator.cpp \
    chart/piechart.cpp \
    chart/scatterchart.cpp \
    chartsetting/backgroundarea.cpp \
    chartsetting/backgroundareagradient.cpp \
    chartsetting/backgroundborder.cpp \
    chartsetting/chartanimation.cpp \
    chartsetting/chartbackground.cpp \
    chartsetting/chartbox.cpp \
    chartsetting/chartforeground.cpp \
    chartsetting/chartlegend.cpp \
    chartsetting/chartmargin.cpp \
    chartsetting/chartsetting.cpp \
    chartsetting/charttitle.cpp \
    chartsetting/colorbutton.cpp \
    chartsetting/conicalgradientparamwidget.cpp \
    chartsetting/foregroundarea.cpp \
    chartsetting/foregroundborder.cpp \
    chartsetting/gradientparamselector.cpp \
    chartsetting/gradientparamselectorwidget.cpp \
    chartsetting/gradientparamspinbox.cpp \
    chartsetting/gradientparamwidget.cpp \
    chartsetting/lineargradientparamwidget.cpp \
    chartsetting/radialgradientparamwidget.cpp \
    charttool/barcharttool.cpp \
    charttool/charttool.cpp \
    charttool/charttoolwidget.cpp \
    charttool/linecharttool.cpp \
    charttool/piecharttool.cpp \
    charttool/scattercharttool.cpp \
    chartview/associatebaraxis.cpp \
    chartview/associatebarmode.cpp \
    chartview/associatebarseries.cpp \
    chartview/associatepiemode.cpp \
    chartview/associatepieseries.cpp \
    chartview/associatexyaxis.cpp \
    chartview/associatexymode.cpp \
    chartview/associatexyseries.cpp \
    chartview/barassociatetable.cpp \
    chartview/barchartbar.cpp \
    chartview/chartbar.cpp \
    chartview/chartview.cpp \
    chartview/lineassociatetable.cpp \
    chartview/linechartbar.cpp \
    chartview/pieassociatetable.cpp \
    chartview/piechartbar.cpp \
    chartview/scatterassociatetable.cpp \
    chartview/scatterchartbar.cpp \
    chartview/screenshoot.cpp \
    communication/communication.cpp \
    communication/communication_connect.cpp \
    communication/query.cpp \
    communication/usbquery.cpp \
    communication/usbconfig.cpp \
    communication/tcpquery.cpp \
    csv/csvexport.cpp \
    csv/csvexport_py.cpp \
    log/log.cpp \
    log/logfixedprocessor.cpp \
    log/logmonitorprocessor.cpp \
    log/logprocessor.cpp \
    log/logterminalprocessor.cpp \
    login/login.cpp \
    login/login_create_account.cpp \
    login/login_retrieve_first.cpp \
    login/login_retrieve_password.cpp \
    login/login_retrieve_second.cpp \
    login/login_retrieve_third.cpp \
    main.cpp \
    mainwindow.cpp \
    messagebox/critical.cpp \
    messagebox/infomation.cpp \
    messagebox/messagebox.cpp \
    messagebox/messagebutton.cpp \
    messagebox/tip.cpp \
    messagebox/warning.cpp \
    monitor/monitor.cpp \
    monitor/monitor_initial.cpp \
    monitor/monitor_label.cpp \
    monitor/monitor_spinbox.cpp \
    seriessetting/barseriessetting.cpp \
    seriessetting/lineseriessetting.cpp \
    seriessetting/pieseriessetting.cpp \
    seriessetting/scatterseriessetting.cpp \
    seriessetting/seriesbar.cpp \
    seriessetting/seriesbarset.cpp \
    seriessetting/seriesbox.cpp \
    seriessetting/seriesinfo.cpp \
    seriessetting/seriespie.cpp \
    seriessetting/seriespieslice.cpp \
    seriessetting/seriesscatter.cpp \
    seriessetting/seriessetting.cpp \
    seriessetting/seriesxy.cpp \
    setting/logperiod.cpp \
    setting/setting.cpp \
    setting/workmodedialog.cpp \
    setting/querypanel.cpp \
    table/table.cpp \
    table/table_impexort.cpp \
    table/table_impexort_concurrent.cpp \
    table/tablecolordialog.cpp \
    table/tablesizedialog.cpp \
    tool/tableview_tool.cpp \
    tool/chartview_tool.cpp \
    tool/tableviewmodel.cpp \
    tool/tool.cpp

HEADERS += \
    axissetting/axisbarcategory.h \
    axissetting/axisbox.h \
    axissetting/axisgrid.h \
    axissetting/axisinfo.h \
    axissetting/axislabel.h \
    axissetting/axisline.h \
    axissetting/axislog.h \
    axissetting/axissetting.h \
    axissetting/axisshade.h \
    axissetting/axistime.h \
    axissetting/axistitle.h \
    axissetting/axisvalue.h \
    chart/barchart.h \
    chart/chart.h \
    chart/chartshowlegend.h \
    chart/chartshowtip.h \
    chart/linechart.h \
    chart/chartdatagenerator.h \
    chart/piechart.h \
    chart/scatterchart.h \
    chartsetting/backgroundarea.h \
    chartsetting/backgroundareagradient.h \
    chartsetting/backgroundborder.h \
    chartsetting/chartanimation.h \
    chartsetting/chartbackground.h \
    chartsetting/chartbox.h \
    chartsetting/chartforeground.h \
    chartsetting/chartlegend.h \
    chartsetting/chartmargin.h \
    chartsetting/chartsetting.h \
    chartsetting/charttitle.h \
    chartsetting/colorbutton.h \
    chartsetting/conicalgradientparamwidget.h \
    chartsetting/foregroundarea.h \
    chartsetting/foregroundborder.h \
    chartsetting/gradientparamselector.h \
    chartsetting/gradientparamselectorwidget.h \
    chartsetting/gradientparamspinbox.h \
    chartsetting/gradientparamwidget.h \
    chartsetting/lineargradientparamwidget.h \
    chartsetting/radialgradientparamwidget.h \
    charttool/barcharttool.h \
    charttool/charttool.h \
    charttool/charttoolwidget.h \
    charttool/linecharttool.h \
    charttool/piecharttool.h \
    charttool/scattercharttool.h \
    chartview/associatebaraxis.h \
    chartview/associatebarmode.h \
    chartview/associatebarseries.h \
    chartview/associatepiemode.h \
    chartview/associatepieseries.h \
    chartview/associatexyaxis.h \
    chartview/associatexymode.h \
    chartview/associatexyseries.h \
    chartview/barassociatetable.h \
    chartview/barchartbar.h \
    chartview/chartbar.h \
    chartview/chartview.h \
    chartview/lineassociatetable.h \
    chartview/linechartbar.h \
    chartview/pieassociatetable.h \
    chartview/piechartbar.h \
    chartview/scatterassociatetable.h \
    chartview/scatterchartbar.h \
    chartview/screenshoot.h \
    communication/communication.h \
    communication/query.h \
    communication/usbquery.h \
    communication/usbconfig.h \
    communication/tcpquery.h \
    core/data_processing.h \
    core/enum.h \
    core/list_files.h \
    core/spic_command.h \
    csv/csvexport.h \
    csv/csvexport_test.h \
    log/log.h \
    log/logfixedprocessor.h \
    log/logmonitorprocessor.h \
    log/logprocessor.h \
    log/logterminalprocessor.h \
    login/login.h \
    login/login_create_account.h \
    login/login_retrieve_password.h \
    mainwindow.h \
    messagebox/critical.h \
    messagebox/infomation.h \
    messagebox/messagebox.h \
    messagebox/messagebutton.h \
    messagebox/tip.h \
    messagebox/warning.h \
    monitor/monitor.h \
    monitor/monitor_label.h \
    monitor/monitor_spinbox.h \
    seriessetting/barseriessetting.h \
    seriessetting/lineseriessetting.h \
    seriessetting/pieseriessetting.h \
    seriessetting/scatterseriessetting.h \
    seriessetting/seriesbar.h \
    seriessetting/seriesbarset.h \
    seriessetting/seriesbox.h \
    seriessetting/seriesinfo.h \
    seriessetting/seriespie.h \
    seriessetting/seriespieslice.h \
    seriessetting/seriesscatter.h \
    seriessetting/seriessetting.h \
    seriessetting/seriesxy.h \
    setting/logperiod.h \
    setting/setting.h \
    setting/workmodedialog.h \
    setting/querypanel.h \
    table/table.h \
    table/tablecolordialog.h \
    table/tablesizedialog.h \
    test/animation_test.h \
    test/pytest_agreement_api.h \
    test/pytest_agreement_test.h \
    test/pytest_intro_api.h \
    test/pytest_intro_test.h \
    test/pytest_print.h \
    test/pytest_print_test.h \
    test/pytest_spec_datatype_api.h \
    test/pytest_spec_funcobj_api.h \
    test/pytest_spec_objlayer_test.h \
    test/pytest_tool_api.h \
    test/pytest_tool_test.h \
    test/pytest_useful_macro_api.h \
    test/pytest_useful_macro_test.h \
    test/concurrent_test.h \
    test/listfiles_test.h \
    test/test.h \
    tool/chartview_tool.h \
    tool/tableview_tool.h \
    tool/tableviewmodel.h \
    tool/tool.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
                images.qrc \
                trans.qrc

DISTFILES += \
                log.txt \
                test/test_function.py \
                test/test_plot.py \
                test/test_python.py \
                test/test_class.py \
                test/pytest.txt \
                csv/csvexport.py

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../AppData/Local/Programs/Python/Python37-32/libs/ -lpython37
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../AppData/Local/Programs/Python/Python37-32/libs/ -lpython37d

INCLUDEPATH += $$PWD/../../AppData/Local/Programs/Python/Python37-32/libs
DEPENDPATH += $$PWD/../../AppData/Local/Programs/Python/Python37-32/libs

INCLUDEPATH += $$PWD/../../AppData/Local/Programs/Python/Python37-32/include
DEPENDPATH += $$PWD/../../AppData/Local/Programs/Python/Python37-32/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../AppData/Local/Programs/Python/Python37-32/libs/libpython37.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../AppData/Local/Programs/Python/Python37-32/libs/libpython37d.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../AppData/Local/Programs/Python/Python37-32/libs/python37.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../AppData/Local/Programs/Python/Python37-32/libs/python37d.lib

#win32:CONFIG(release, debug|release): LIBS += ./libs/ -lpython37
#else:win32:CONFIG(debug, debug|release): LIBS += ./libs/ -lpython37d

#INCLUDEPATH += ./libs
#DEPENDPATH += ./libs

#INCLUDEPATH += ./include
#DEPENDPATH += ./include

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += ./libs/libpython37.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += ./libs/libpython37d.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += ./libs/python37.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += ./libs/python37d.lib
