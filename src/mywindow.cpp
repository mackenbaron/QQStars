#include "mywindow.h"
#include "utility.h"
#include <QDebug>
#include <QQmlComponent>
#include <QApplication>
#include <QPainter>
#include <QPainterPath>
#ifdef Q_OS_WIN
#include <winuser.h>
#endif

MyWindow::MyWindow(QQuickWindow *parent) :
    QQuickWindow(parent), backingStore(0)
{
    m_noBorder = false;
    m_windowStatus = StopCenter;
    m_topHint = false;
    old_topHint=false;
    m_noNotifyIcon = false;
}

bool MyWindow::topHint() const
{
    return m_topHint;
}

QUrl MyWindow::windowIcon()
{
    return m_windowIcon;
}

void MyWindow::setWindowIcon(QUrl icon)
{
    if( icon!=m_windowIcon ){
        QString str = icon.toString ();
        if( str.mid (0, 3) == "qrc")
            str = str.mid (3, str.count ()-3);
        setIcon (QIcon(str));
        m_windowIcon = icon;
        emit windowIconChanged ();
    }
}

bool MyWindow::noBorder()
{
    return m_noBorder;
}

void MyWindow::setNoBorder(bool isNoBroder)
{
    if( isNoBroder!=m_noBorder ) {
        m_noBorder = isNoBroder;
        if(!isVisible ()) {
            if( isNoBroder )
                setFlags (flags ()|Qt::FramelessWindowHint);
            else
                setFlags (flags ()&~Qt::FramelessWindowHint);
        }else{
            if( isNoBroder )
                setFlags (flags ()|Qt::FramelessWindowHint);
            else
                setFlags (flags ()&~Qt::FramelessWindowHint);
        }
        
        emit noBorderIconChanged();
    }
}

MyWindow::Status MyWindow::windowStatus()
{
    return m_windowStatus;
}

void MyWindow::setWindowStatus(MyWindow::Status new_status)
{
    if( new_status!=m_windowStatus ) {
        if( new_status == BerthPrepare&&m_windowStatus!=StopCenter ) {
            setTopHint (old_topHint);
        }else if(new_status!=StopCenter&&new_status!=BerthPrepare){
            old_topHint = topHint ();
            setTopHint (true);//设置窗口为最前端
        }
        m_windowStatus = new_status;
        emit windowStatusChanged ();
    }
}

int MyWindow::borderLeft()
{
#ifdef Q_OS_WIN
    return 0;
#elif defined(Q_OS_MACX)
    return 0;
#elif defined(Q_OS_LINUX)
    return 65;
#else
    return 0;
#endif
}

int MyWindow::borderRight()
{
#ifdef Q_OS_WIN
    return QApplication::screens ()[0]->size ().width ();
#elif defined(Q_OS_MACX)
    return QApplication::screens ()[0]->size ().width ();
#elif defined(Q_OS_LINUX)
    return QApplication::screens ()[0]->size ().width ();
#else
    return QApplication::screens ()[0]->size ().width ();
#endif
}

int MyWindow::borderTop()
{
#ifdef Q_OS_WIN
    return 0;
#elif defined(Q_OS_MACX)
    return 0;
#elif defined(Q_OS_LINUX)
    return 25;
#else
    return 0;
#endif
}


void MyWindow::setTopHint(bool arg)
{
    if (m_topHint != arg) {
        m_topHint = arg;
        if( arg ){
            setFlags (flags ()|Qt::WindowStaysOnTopHint);
#ifdef Q_OS_LINUX
            if(isVisible()){
                hide();
                show();
            }
#endif
        }else{
            setFlags (flags ()&~Qt::WindowStaysOnTopHint);
#ifdef Q_OS_LINUX
            if(isVisible()){
                hide();
                show();
            }
#endif
#ifdef Q_OS_WIN
            SetWindowPos ((HWND)this->winId (),HWND_NOTOPMOST,0,0,width(),height(),SWP_NOSIZE|SWP_NOMOVE);
#endif
        }
        emit topHintChanged(arg);
    }
}

void MyWindow::setNoNotifyIcon(bool arg)
{
    if ( m_noNotifyIcon != arg ) {
        m_noNotifyIcon = arg;
        if( arg )
            setFlags (flags ()&~Qt::Tool);
        else{
            setFlags (flags ()|Qt::Tool);
        }
        emit noNotifyIconChanged(arg);
    }
}
