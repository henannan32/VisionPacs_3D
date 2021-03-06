#ifndef IMAGEWND_H
#define IMAGEWND_H

#include <QWidget>


class CHsImage;
class CHsNormalMprMaker;
class WorkZone;
class OperateMprLines;

namespace Ui {
class ImageWnd;
}

class ImageWnd : public QWidget
{
    Q_OBJECT

public:
    explicit ImageWnd(QWidget *parent = 0);
    ~ImageWnd();

private:
    Ui::ImageWnd *ui;

	//聚焦标记
	bool m_bFocused; 

	//计算图像在窗口内显示的rc
	RECT CalDisplayRect(CHsImage*pImg);//RECT DlgRc,

	int m_nInteractionState;//鼠标操作的状态

	//鼠标隐藏计数
	QPoint m_PrePoint;//移动、缩放、WC等,需要鼠标移动控制效果

	HSRECT m_fImgRc;//精确的图像区域

	//确定窗口图像类型
	int m_nImgWndType;

	//普通MPR切割类
	CHsNormalMprMaker *m_pNormalMaker;

	//本窗口图像总数
	int m_nImgNum;

	//初始化角落信息
	void RefreshCornorInfoWidget();
	bool m_bInitCorInfo;
	
	//角落信息处理
	typedef struct _ROWITEM
	{
		QString sValue;//填充值之后
		QString sType;
		_ROWITEM()
		{
			sValue = "";
			sType = "Normal";
		}
	}ROWITEM;

	typedef struct _QEDITITEM
	{
		QString sName;
		QString sPos;
		QLineEdit *qEdit;
		QLabel *qPreLabel;
		QLabel *qNextLabel;
		_QEDITITEM()
		{
			sName = "";
			sPos = "";
			qEdit = NULL;
			qPreLabel = NULL;
			qNextLabel = NULL;
		}
	}QEDITITEM;

	vector<QEDITITEM> m_vCornorEdit;
	int ArrangeCorinfo(CORNORINFO corInfo,map<int,ROWITEM> &mapRow);
	void InitNormalCorInfo();
	void InitPosCorInfo();
	void ReSetPosCorInfo(QSize deltaSize);

	//标签集合，方便调整位置
	map<QLabel*,QString> m_mapInfoLabel;
	map<QLineEdit *, QString> m_mapInfoEdit;

	//当前图像序数
	int m_nCurImgIndex;

	//图像
	CHsImage *m_pImg;

	//操作线
	OperateMprLines *m_pOperateLines;
protected:
	QPixmap *m_pPixmap;
	QImage *m_pQImage;

	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent* size);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	
public:
	
	unsigned int m_nLeftButtonInteractionStyle;
	unsigned int m_nRightButtonInteractionStyle;

public:
	//设置本窗体是否为聚焦窗口
	void SetFocuse(bool bFocused){ m_bFocused = bFocused; } 

	//获得本窗口是否为聚焦窗口
	bool IsFocused(){ return m_bFocused; }	

	//判断本窗口内是否有图像显示
	bool IsImgEmpty();

	//设置本窗口内显示图像
	int SetImage(CHsImage *pImg);

	void ConvertCoord(long *x1, long *y1, long *x2, long *y2, bool bFromHwdToImg);

	//设置操作
	void SetOperate(QString operate);

	//窗口左边转图像坐标
	POINT ConvertWndToImg(RECT ImgRcOnWnd, long nImgW, long nImgH, QPoint &pt);

	long m_nCurW, m_nCurC;

	void ***m_p3DArray;
	vtkSmartPointer<vtkImageData> m_p3DImgData;
	vector<CHsImage*> m_vOriImg;

	void GetImgNumAndWndType(QString sWndName, int nOriImgType);

	//计算本窗口显示图像
	int CalcAndShowNormalImg(QString sWndName, int nOriImgType, int iImgIndex,int iSlice);

	void SetCurImageIndex(int nIndex) { m_nCurImgIndex = nIndex; }

	//获得操作线
	OperateMprLines *GetOperateLine() { return m_pOperateLines; }

	//获得图像
	CHsImage *GetImage() { return m_pImg; }

	//设置MPR模式
	void SetMprMode(QString sModeName);


signals:
	void SendImageNum(int);
	void ImageIndexChange(int);
	void ImageThickChange(double);

private slots:
	void OnEditTextChanged(const QString &sText);
	void OnEditFinished();
	void OnMprLinesShow(bool isShow);
	
	
};

#endif // IMAGEWND_H
