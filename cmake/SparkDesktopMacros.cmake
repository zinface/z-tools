# SparkDesktopMacros.cmake

macro(spark_desktop_macros)
    set(APP_NAME              ${ARGV0})
    set(APP_NAME_ZH_CN        ${ARGV1})
    set(APP_COMMENT           ${ARGV2})
    set(APP_TYPE              ${ARGV3})
    set(APP_EXECUTE_PATH      ${ARGV4})
    set(APP_EXECUTE_ICON_PATH ${ARGV5})
    set(APP_CATEGORIES        ${ARGV6})
    set(APP_MIME_TYPE         ${ARGV7})
    configure_file(cmake/spark-desktop.desktop.in
        ${CMAKE_BINARY_DIR}/${ARGV0}.desktop
    )
    set(DESKTOP_FILE ${CMAKE_BINARY_DIR}/${ARGV0}.desktop)
endmacro(spark_desktop_macros)

# include(cmake/SparkDesktopMacros.cmake)
# 内容默认应用名称: Name= 应与项目名称相同
# spark_desktop_macros(
    # 应用名称: Name=
    # 应用名称: Name[zh_CN]=
    # 应用说明: Comment=
    # 应用类型: Type=
    # 执行程序: Exec=
    # 图标路径: Icon=
    # 应用分类: Category=
    # MIME类型: MimeType=
# )

# configure_file(<input> <output>
#                [NO_SOURCE_PERMISSIONS | USE_SOURCE_PERMISSIONS |
#                 FILE_PERMISSIONS <permissions>...]
#                [COPYONLY] [ESCAPE_QUOTES] [@ONLY]
#                [NEWLINE_STYLE [UNIX|DOS|WIN32|LF|CRLF] ])

# install(FILES ${APP_NAME}.desktop
#     DESTINATION /usr/share/applications
# )