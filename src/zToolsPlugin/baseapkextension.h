#ifndef BASEAPKEXTENSION_H
#define BASEAPKEXTENSION_H

#include <extensions.h>
#include <../ApkManager/apkmanager.h>

class BaseApkExtension : public IExtension
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID IExtension_IID)
    Q_INTERFACES(IExtension)
public:
    BaseApkExtension() {};

    // IExtension interface
public:
    /**
     * @brief PluginName 插件的名称
     * @return
     */
    QString PluginName() override {
        return ">BaseApkExtension";
    };
    /**
     * @brief PluginxAlwaysDisplay 插件可持续显示
     * @return
     */
    bool PluginxAlwaysDisplay() override {
        return true;
    };
    /**
     * @brief pluginAutoLoaderProcess 插件的加载控件来源
     * @return
     */
    QWidget *pluginAutoLoaderProcess() override {
        return new ApkManager;
    };
};

#endif // BASEAPKEXTENSION_H
