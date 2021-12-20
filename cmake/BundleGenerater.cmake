cmake_minimum_required(VERSION 3.0.0)

set(CPACK_GENERATOR "Bundle")
set(PART_NAME "FileTransfer - v${PROJECT_VERSION}")

#生成的包的名称。这在macOS Finder中显示为包名。必需的。
set(CPACK_BUNDLE_NAME "${PROJECT_NAME}")

#将用于生成的绑定包的macOS属性列表(.plist)文件的路径。
#这假设调用者已经生成或指定了他们自己的信息。plist文件。必需的。
set(CPACK_BUNDLE_PLIST "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.plist")

#macOS图标文件的路径，该文件将用作生成的绑定包的图标。
#这是在绑定包的macOS Finder中出现的图标，在打开绑定包时出现在macOS dock中。必需的。
set(CPACK_BUNDLE_ICON "${CMAKE_SOURCE_DIR}/resources/z-Tools.icns")

#启动脚本的路径。
#这是一个可执行文件或脚本的路径，当终端用户在macOS Finder中双击生成的包时，该脚本就会运行。可选的。
set(CPACK_BUNDLE_STARTUP_COMMAND)

#您的Apple为应用程序提供的代码签名证书的名称。
#名称通常采用“开发者ID应用程序:[name]”或“第三方Mac开发者应用程序:[name]”的形式。
#如果没有设置此变量，则不会对应用程序进行签名。
set(CPACK_BUNDLE_APPLE_CERT_APP)

#属性列表(.plist)文件的名称，该文件包含用于沙箱应用程序的Apple授权。
#提交到macOS App Store时需要这个文件。
set(CPACK_BUNDLE_APPLE_ENTITLEMENTS)

#您希望被签署的附加文件的列表。
#您不需要列出主应用程序文件夹或主可执行文件。你应该列出你的应用程序包中包含的所有框架和插件。
set(CPACK_BUNDLE_APPLE_CODESIGN_FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}")

#将传递给codesign的附加参数。缺省值:——deep -f
set(CPACK_BUNDLE_APPLE_CODESIGN_PARAMETER)

#使用Apple cert签名应用程序的codesign(1)命令的路径。
#这个变量可以被用来覆盖自动检测命令(或者指定它的位置，如果自动检测不能找到它)。
set(CPACK_COMMAND_CODESIGN)


file(WRITE "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.plist" "\
<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<plist version=\"1.0\">
<dict>
    <key>CFBundleDisplayName</key>
    <string>z-Tools-${PART_NAME}</string>
    <key>CFBundleExecutable</key>
    <string>${PROJECT_NAME}</string>
    <key>CFBundleIconFile</key>
    <string>${PROJECT_NAME}.icns</string>
    <key>CFBundleName</key>
    <string>${PART_NAME}</string>
    <key>CFBundlePackageType</key>
    <string>APPL</string>
</dict>
</plist>
")

install(PROGRAMS
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}

    DESTINATION ../MacOS
)

include(CPack)

# You will need to manually execute 'macdeployqt project.app -dmg' to deploy the dependencies and generate the '*.dmg' files
