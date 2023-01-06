CPUS=$(shell nproc)
CALENDAR=$(shell date '+%Y%m%d')
OSID=$(shell lsb_release -si)
OSRELEASE=$(shell lsb_release -sr)
SUFFIX=
ifneq ("$(OSID)", "")
SUFFIX=_$(OSID)$(OSRELEASE)
endif

PROJECT_NAME=z-tools-speedwidget

all:
	mkdir -p build
	cd build && cmake ..
	cd build && make -j$(CPUS)

run: all
	exec $(shell find build/ -maxdepth 1 -type f -executable | grep $(PROJECT_NAME))

debug: 
	mkdir -p build
	cd build && cmake -DCMAKE_BUILD_TYPE=Debug ..
	cd build && make -j$(CPUS)

release:
	mkdir -p build
	cd build && cmake -DCMAKE_BUILD_TYPE=Release -DPACKAGE_SUFFIX="$(SUFFIX)" ..
	cd build && make -j$(CPUS)

package: release
	cd build && make package
	tree build/_CPack_Packages/Linux/DEB/$(PROJECT_NAME)-*
	dpkg-deb --contents build/$(PROJECT_NAME)_*$(CALENDAR)*$(SUFFIX).deb
	# cd build/_CPack_Packages/Linux/DEB/$(PROJECT_NAME)_*$(CALENDAR)*$(SUFFIX).deb && find .

builddeps:
	cd build && make builddeps

cpus:
	@echo "CPU数量: $(CPUS)"

copytosource:package
	cp build/$(PROJECT_NAME)_*$(CALENDAR)*.deb .

# 进入 qdebug 模式，在 deepin 中默认被禁用，可 env | grep QT 查看，并在 /etc/X11/Xsession.d/00deepin-dde-env 配置中已定义
# 1. 禁止 qt 的 debug 打印: qt.*.debug=false
# 	qt.qpa.input.events
#	qt.qpa.events
# 2. 禁止 dtk 的 debug 打印: dtk.*.debug=false
# 	dtk.dpluginloader
# 3. 禁止 qtcreator 本身的 debug 打印
# 	qtc.autotest.testcodeparser
#	qtc.clangbackend.server
#	...
# 4. 关闭其它的太麻烦了，直接只启用本地 debug
#	.debug=true
enter-qdebug-mode:
	# 进入新的 bash 环境
	@# export QT_LOGGING_RULES=".debug=true; qt.*.debug=false; dtk.*.debug=false; dde.*.debug=false; qtc*=false; " && bash
	export QT_LOGGING_RULES=".debug=true" && bash