cmake_minimum_required(VERSION 3.5.1)

# translator_qt5 _qmvar [... *.ts]
macro(translator_qt5 _qmvar)
    # set(SPARK_TRANSLATIONS_ARGN ${ARGN})
    # file(GLOB SPARK_TRANSLATIONS ${SPARK_TRANSLATIONS_ARGN})
    # qt5_add_translation(SPARK_QM_TRANSLATIONS 
        # ${SPARK_TRANSLATIONS})
    
    set(${_qmvar}_ARNG ${ARGN})
    file(GLOB ${_qmvar}_TS_FILES ${${_qmvar}_ARNG})
    
    find_package(Qt5LinguistTools)
    qt5_add_translation(${_qmvar}
        ${${_qmvar}_TS_FILES})

    set(SPARK_QM_TRANSLATIONS ${_qmvar})

    spark_debug_message("> QT Translation: ${_qmvar}")
    file(WRITE ${CMAKE_BINARY_DIR}/${_qmvar} "")
    foreach(item IN LISTS ${_qmvar})
        file(APPEND ${CMAKE_BINARY_DIR}/${_qmvar} "${item}\n")
        spark_debug_message("      ${item}")
    endforeach(item IN LISTS ${_qmvar})
    
    # 注意，必须将 SPARK_QM_TRANSLATIONS 或 ${_qmvar} 加入到 add_executable 参数中才能在编译时生成只有原文的ts文件
    
    # qt5_create_translation
        # ts文件会在 make clean 或重新编译的时候一并被删除，再编译的时候生成全新的ts（原有的翻译会丢失，万分注意！!）
    
    # qt5_add_translation
        # 此宏比较稳定
endmacro(translator_qt5 _qmvar)


# translator_qt6 _qmvar [... *.ts]
macro(translator_qt6 _qmvar)
    # todo
endmacro(translator_qt6 _qmvar)

# 冗余的 translator_qt5 或 qt6 的处理逻辑
macro(_handle_spark_translator_qt_macro _outvar)
    if(SPARK_FIND_QT5)
        translator_qt5(${_outvar} ${ARGN})
    endif(SPARK_FIND_QT5)

    if(SPARK_FIND_QT6)
        translator_qt6(${_outvar} ${ARGN})
    endif(SPARK_FIND_QT6)
endmacro(_handle_spark_translator_qt_macro _outvar)

# translator_qt _qmvar [... *.ts | match]
macro(translator_qt)
    _handle_spark_translator_qt_macro(${ARGN})
endmacro(translator_qt)
