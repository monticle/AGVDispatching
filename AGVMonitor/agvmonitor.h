#ifndef AGVMONITOR_H
#define AGVMONITOR_H
#include "mygraphicsview.h"
#include "mydatabase.h"
#include "myqsqltablemodel.h"
#include "myserialport.h"
#include "dialog_viewstatus.h"
#include "dialog_agvmanagement.h"
#include "ui_dialog_agvmanagement.h"
#include <QMainWindow>
#include <QStandardItemModel>
#include <QGraphicsScene>
#include <QDomNode>
#include <QPushButton>
#include <QSqlTableModel>
#include <QEvent>
#include <QTimer>
namespace Ui {
class AGVMonitor;
}



struct RoutePoint
{
    int node_num;
    QString RFID_num;
    double pos_x; //保存在路径段中
    double pos_y; //保存在路径段中
    QVector<int> line_enter;
    QVector<int> line_exit;
    int node_type;

};

struct Route
{
    int path_num;
    int start_num;
    int end_num;
    double angle;
    double length;
    int enable_dir;
    QVector<int> contain_station;
    int capacity;
    QPointF start_pos; //显示图元坐标
    QPointF end_pos;
    QPointF old_start;
    QPointF old_end;

};

struct StationPoint
{
    int node_num;
    QString RFID_num;
    int attach_num;
    double length;
    int node_type;
    double PosX;
    double PosY;
};

struct AGVInfo
{
    int agvNo;
    double PosX;
    double PosY;
};

class AGVMonitor : public QMainWindow
{
    Q_OBJECT

public:
    explicit AGVMonitor(QWidget *parent = nullptr);
    ~AGVMonitor();

private:
    Ui::AGVMonitor *ui;

    /*********表格模型**********/
    //任务
    MyQSqlTableModel*  Model_taskExecuting;
    MyQSqlTableModel*  Model_taskNew;
    MyQSqlTableModel*  Model_taskDispatch;
    MyQSqlTableModel*  Model_taskComplete;
    //异常
    MyQSqlTableModel*  Model_taskError;
    MyQSqlTableModel*  Model_agvError;
    MyQSqlTableModel*  Model_deviceError;
    //AGV
    MyQSqlTableModel*  Model_agvStatus;
    MyQSqlTableModel*  Model_agvStatics;
    //物料
    MyQSqlTableModel*  Model_materialStatus;

    QGraphicsScene* m_myScene;
    MyGraphicsView* m_myView;

    //结构体链表
    QList<Route> m_list_route;
    QList<RoutePoint> m_list_routePoint;
    QList<StationPoint> m_list_stationPoint;
    QList<AGVInfo> m_list_agvInfo;

    //样式
    QPen RoutePen;
    QPen ArrowPen;
    QBrush PointBrush;
    QBrush StationBrush;
    QBrush LiftAgvBrush;
    QBrush PullAgvBrush;
    QBrush LatentAgvBrush;

    //数据库
    MyDatabase* m_myDatabase;

    //测试
    QPushButton* Btn_test;

    //串口
    MySerialPort* m_mySerialPort;
    int count_serialPort = 0;
    int serialPortStatus = 0;//第一次打开为0，1表示show,关闭自动隐藏

    //显示监视窗口
    QTimer* timer_display;
    QString last_Type; //避免重复打开查看窗口
    int last_TypeNo;

    //限制最大记录数
    QTimer* timer_record;

    //agv移动
    QTimer* timer_agvPos;
    int count = 0;

    //地图映射偏移
//    double MapTopLeftX; //电子地图模块参考点X
//    double MapTopLeftY;
    double offsetX;
    double offsetY;

    //刷新呼叫区
    QTimer* timer_flushCall;


protected:
    void createModel();
    void initStyle();
    void createConnection();
    bool eventFilter(QObject *obj,QEvent *event);
//    void mousePressEvent(QGraphicsSceneMouseEvent *event);
//    void GraphicsSceneMousePress(QEvent*);


    //读取xml
    int getXmlValue(const QDomNode &node,QString type);
    double getXmlValueF(const QDomNode &node,QString type);
    QString getXmlRFID(const QDomNode &node,QString type);
    QVector<int> getXmlValueList(const QDomNode &node,QString type);

    //地图映射
    void mapOffset(QList<QPointF>);

    //路网监控
    void updateRoad(QString type,int number);

    //AGV监控




private slots:
    void slot_importMap();
    void slot_Btn_test();
    void slot_serialPort();
    void slot_timer_display();
    void slot_timer_record(); //考虑在串口处理
    void slot_timer_agvPos();
    void slot_timer_flushCall();
    void slot_agvAccount();
    void slot_agvSettings();


};

#endif // AGVMONITOR_H
