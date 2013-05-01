#include <QApplication>
#include "mainwindow.h"
#include <QTextCodec>
#include "widgettest.h"
#include "wavewidget/wavewidget.h"
#include "widgetwaveshow.h"
#include <QFile>
#include <QDebug>
int main(int argc, char *argv[])
{

    QFile f("/proc/stat");
    QTextStream s(&f);
    qDebug() << s.readAll();
    QApplication a(argc, argv);

    //设置常量字符串和QString的字符编码
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    //设置QObject::tr方法里面时的编码
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    //设置本地读取文件流时的编码，也用于debug显示数据到屏上的编码
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

    QFont font;
    font.setPixelSize(15); //
    font.setFamily(("simsun"));
    font.setBold(false);
    a.setFont(font);

    MainWindow w;
    //w.showMaximized();
    w.setWindowFlags(w.windowFlags() &~Qt::WindowMinimizeButtonHint);

    w.show();

//    widgetTest testShow;
//    testShow.show();

//    WaveWidget t;
//    t.show();

//    widgetWaveShow *d = new widgetWaveShow;
//    d->show();





    return a.exec();
}
