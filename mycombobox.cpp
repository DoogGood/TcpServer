#include "mycombobox.h"
#include <QMouseEvent>





void MyComboBox::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton){
        //触发被clicked信号
        emit my_comboBox_clicked();

    }
    //调用基类的同名事件处理函数,处理这个控件本身的一些操作
    QComboBox::mousePressEvent(e);


}


MyComboBox::MyComboBox(QWidget *parent):QComboBox (parent)
{

}
