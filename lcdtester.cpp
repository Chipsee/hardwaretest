#include "lcdtester.h"
#include "ui_lcdtester.h"
#include <QTimer>
#include <QPainter>
#include <QScreen>
#include <QGuiApplication>
#include <QDebug>

LCDTester::LCDTester(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LCDTester),index(0)
{
    ui->setupUi(this);

    // Initialize the list of colors
    // colors[6] is to draw a white rectangle frame and use black background
    // colors[7] is dummy and will replace by one color bar
    colors = {Qt::red, Qt::green, Qt::blue, Qt::white, Qt::gray, Qt::black, Qt::black, Qt::black};

    // Enable touch events for this dialog
    setAttribute(Qt::WA_AcceptTouchEvents);

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    lcdwidth = screenGeometry.width();
    lcdheight = screenGeometry.height();
}

// Override the touchEvent method to handle touch events
bool LCDTester::event(QEvent *event)
{
    // Check if the event is a touch event
    if (event->type() == QEvent::TouchBegin ||
            event->type() == QEvent::MouseButtonPress )
    {
        index = (index + 1) % colors.size(); // Increment the index and wrap around

        // Will close after one loop, now the index is 0, so use !index
        if(!index)
            close();

        update(); // Call the update method to repaint the dialog

        // Accept the touch event and return true
        event->accept();
        return true;
    }

    // Call the base class implementation for other events and return its result
    return QDialog::event(event);
}

// Override the paintEvent method to draw the color on the dialog
void LCDTester::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event); // Ignore the event parameter

    // Create a painter object to draw on the dialog
    QPainter painter(this);

    // Color bar
    QLinearGradient linearGradient(QPointF(0, 0), QPointF(lcdwidth, lcdheight));
    linearGradient.setColorAt(0.0, Qt::red);
    linearGradient.setColorAt(0.5, Qt::green);
    linearGradient.setColorAt(1.0, Qt::blue);
    QBrush brush(linearGradient);

    if(index==7){
        painter.setBrush(brush);
    }else{
        // Set the painter's brush to the current color
        painter.setBrush(colors[index]);
    }

    if(index==6){
        QPen pen;
        pen.setColor(Qt::white);
        pen.setWidth(10);
        painter.setPen(pen);
    }

    // Draw a rectangle that fills the entire dialog area
    painter.drawRect(rect());

    // Draw the color name in white on top of the rectangle
    //painter.setPen(Qt::white);
    //painter.drawText(rect(), Qt::AlignCenter, colors[index].name());
}

void LCDTester::LCDTesterShow()
{
    this->showFullScreen();
}

LCDTester::~LCDTester()
{
    delete ui;
}
