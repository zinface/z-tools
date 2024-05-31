#ifndef __ScreenUtils__H__
#define __ScreenUtils__H__

#include <QWidget>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QScreen>
#include <QRect>

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE
class ScreenUtils : public QObject
{
    Q_OBJECT
public:
    explicit ScreenUtils(QObject *parent=nullptr);

    /**
     * @brief  移动到指定的屏幕中心
     * @note   
     * @param  &window: 
     * @param  *screen: 
     * @retval None
     */
    static void moveScreenCenter(QWidget &window, QScreen *screen);
    static void moveScreenTopRight(QWidget &window, QScreen *screen);

    /**
     * @brief  移动到主屏幕中心
     * @note   
     * @param  &window: 
     * @retval None
     */
    static void movePrimaryScreenCenter(QWidget &window);
    /**
     * @brief  移动到屏幕中心
     * @note   
     * 1. 如果只有一个屏幕，将会移动到主屏幕
     * 2. 如果有多个屏幕，将会移动到启动它的屏幕
     * @param  &window: 
     * @retval None
     */
    static void moveCenter(QWidget &window);

    /**
     * @brief  移动到鼠标所在的屏幕中心
     * @note   这是常规性的方式
     * * 如果不做任何窗口移动，窗口管理器将会使窗口自动出现鼠标所在屏幕的任何位置
     * @param  &window: 
     * @retval None
     */
    static void moveCenterForCursor(QWidget &window);


    void moveMouseCenter(QWidget &wm);
};

#endif  //!__ScreenUtils__H__