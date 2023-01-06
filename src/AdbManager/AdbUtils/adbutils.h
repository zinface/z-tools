#ifndef ADBUTILS_H
#define ADBUTILS_H

#include <QString>
#include <QStringList>

class AdbUtils
{
public:
    static void StopServer();
    static QStringList GetDevices();
    static QString GetBrand(const QString& deviceId);
    static QString GetModel(const QString& deviceId);
    static QString GetManufacturer(const QString& deviceId);
    static QString GetAndroidApiLevel(const QString& deviceId);
    static QString GetAndroidVersion(const QString& deviceId);
    static QString GetAndroidWmSize(const QString& deviceId);
    static QStringList GetAndroidPackages(const QString& deviceId, const QString& tag, const QString& keyword);

    enum PACKAGE_MGT{
//        ONLY_APK         ,   //-f	显示应用关联的 apk 文件
        ONLY_ENABLED     ,   //-e	只显示 enabled 的应用
        ONLY_DISABLED    ,   //-d	只显示 disabled 的应用
        ONLY_SYSTEM      ,   //-s	只显示系统应用
        ONLY_THRID       ,   //-3	只显示第三方应用
//        ONLY_INSTALLER   ,   //-i	显示应用的 installer
//        ONLY_UNINSTALLER ,   //-u	包含已卸载应用
    };
//    static QString ActiveUnit(const QString& deviceId);
//    static QString RunAutoScript(const QString& deviceId);
//    static QString GetMode(const QString& deviceId);
    static QString RunCommand(const QString& commandLine);
private:
//    static QString RunCommand(const QString& commandLine);
};

#endif // ADBUTILS_H

//[ro.build.version.release]: [10]
//[ro.build.version.sdk]: [29]
//[ro.build.version.security_patch]: [2020-07-01]
//[service.adb.tcp.port]: [9999]

//Brand 品牌: XiaoMi
//Model 型号: Mi 10
//ApiLevel SDK 版本：29
//release Android版本：10(Q)
//adb 端控：9999

//Android 1.0	1	发条机器人	2008-09
//Android 1.1	2	Petit Four 花式小蛋糕	2009-02
//Android 1.5	3	Cupcake 纸杯蛋糕	2009-04
//Android 1.6	4	Donut 甜甜圈	2009-09
//Android 2.0	5	Éclair 松饼	2009-10
//Android 2.0.1	6	Éclair 松饼	2009-10
//Android 2.1	7	Éclair 松饼	2009-10
//Android 2.2-2.2.3	8	Froyo 冻酸奶	2010-05
//Android 2.3-2.3.2	9	Gingerbread 姜饼	2010-12
//Android 2.3.3-2.3.7	10	Gingerbread 姜饼	2010-12
//Android 3.0	11	Honeycomb 蜂巢	2011-02
//Android 3.1	12	Honeycomb 蜂巢	2011-02
//Android 3.2	13	Honeycomb 蜂巢	2011-02
//Android 4.0-4.0.2	14	Ice Cream Sandwich 冰激凌三明治	2011-10
//Android 4.0.3-4.0.4	15	Ice Cream Sandwich 冰激凌三明治	2011-10
//Android 4.1	16	Jelly Bean 糖豆	2012-07
//Android 4.2	17	Jelly Bean 糖豆	2012-07
//Android 4.3	18	Jelly Bean 糖豆	2012-07
//Android 4.4	19	KitKat 奇巧巧克力棒	2013-10
//Android 4.4W	20	KitKat with wearable extensions 奇巧巧克力	2013-10
//Android 5.0-5.0.2	21	Lollipop 棒棒糖	2014-11
//Android 5.1	22	Lollipop 棒棒糖	2014-11
//Android 6.0-6.1	23	Marshmallow 棉花糖	2015-10
//Android 7.0	24	Nougat 牛轧糖	2016-08
//Android 7.1	25	Nougat 牛轧糖	2016-10
//Android 8.0	26	Oreo 奥利奥	2017-08
//Android 8.1	27	Oreo 奥利奥	2017-12
//Android 9	    28	Pie -	2018-05
//Android 10	29	Q -

//作者：Jinbeen
//链接：https://www.jianshu.com/p/0a3b972b85a0
//来源：简书
//著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。


// 版本号                  英文名                 API等级  发布时间              中文名  
// Android 1.0            -                     1       2008-9-23            无
// Android 1.1            -                     2       2009-2-9             无
// Android 1.5            Cupcake               3       2009-4-30            纸杯蛋糕
// Android 1.6            Donut                 4       2009-9-15            甜甜圈
// Android 2.1            Eclair                7       2009-10-26           闪电泡芙
// Android 2.2            Froyo                 8       2010-5-20            冻酸奶
// Android 2.3            Gingerbread           9       2010-12-7            姜饼
// Android 2.3.3          Gingerbread           10      2011-2-9             姜饼
// Android 3.0            Honeycomb             11      2011-2-22            蜂巢
// Android 3.1            Honeycomb             12      2011-5-10            蜂巢
// Android 3.2            Honeycomb             13      2011-7-15            蜂巢
// Android 4.0            Ice cream Sandwich    14      2011-10-19           冰淇淋三明治
// Android 4.0.3-4.0.4    Ice cream Sandwich    14                           冰淇淋三明治
// Android 4.1            Jelly Bean            16      2012-6-28            果冻豆
// Android 4.2            Jelly Bean            17      2012-10-30           果冻豆
// Android 4.3            Jelly Bean            18      2013-7-25            果冻豆
// Android 4.4            KitKat                19      2013-10-31           雀巢巧克力
// Android 5.0            Lollipop              21      2014-10-16           棒棒糖
// Android 5.1            Lollipop              22      2015-3-10            棒棒糖
// Android 6.0            Marshmallow           23      2015-8-18            棉花糖
// Android 7.0            Nougat                24      2016-8-22            牛轧糖
// Android 7.1            Nougat                25      2016-10-4            牛轧糖
// Android 8.0            Oreo                  26      2017-8-21            奥利奥
// Android 8.1            Oreo                  27      2017-12-5            奥利奥
// Android 9.0            Pie                   28       2018-8-6            派
// Android 10.0           Q                     29      2019-9-3             抛弃糖果命名
// Android 11.0           R                     30      2020-9-8             抛弃糖果命名
// Android 12.0           S                     31      2021-10-4            抛弃糖果命名
// Android 13.0           T                     32      2022-10...           抛弃糖果命名
// ————————————————
// 版权声明：本文为CSDN博主「Joven0」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
// 原文链接：https://blog.csdn.net/Joven0/article/details/105022472