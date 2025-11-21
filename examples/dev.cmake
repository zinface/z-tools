# find examples/apk-icon-show | grep -E '.h$|.cpp$|.ui$' | sort
add_executable(example-apk-icon-show
    examples/apk-icon-show/apk-icon-show.cpp
    examples/apk-icon-show/apkiconshowmainwindow.h
    examples/apk-icon-show/apkiconshowmainwindow.cpp
    examples/apk-icon-show/apkiconshowmainwindow.ui
)
target_link_libraries(example-apk-icon-show Qt${QT_VERSION_MAJOR}::Widgets)

