#表示"当前目录下没有需要编译的文件，请向子目录深入"，
#是告诉编译器继续向目录深处递归的一种规定写法
#编译完当前目录的Makefile，不会再编译子目录的Makefile
#当前目录没有Makefile, 则编译子目录中的Makefile
#include $(call all-named-subdir-makefiles,$(AO_SAMPLE_MODULE))

#表示"当前目录下没有需要编译的文件，请向子目录深入"，
#是告诉编译器继续向目录深处递归的一种规定写法
#编译完当前目录的Makefile，会再编译子目录的Makefile
#不管当前目录没有Makefile, 都会编译指定的子目录中的Makefile
#include $(call call all-makefiles-under,$(AO_SAMPLE_MODULE))
#"BUILD_EXECUTABLE"
#"BUILD_SHARED_LIBRARY"
#"BUILD_STATIC_LIBRARY"

Makefile_Target = "BUILD_SHARED_LIBRARY"
CPU_TYPE = "Hi3798MV100"
$(warning Step into Android.mk)
$(warning "$(Makefile_Target) for $(CPU_TYPE)")
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
include $(SDK_DIR)/Android.def
ifeq ("BUILD_EXECUTABLE", $(Makefile_Target))
LOCAL_MODULE:=demo_cutest
else
LOCAL_MODULE:=cutest
endif
ALL_DEFAULT_INSTALLED_MODULES += $(LOCAL_MODULE)


LOCAL_MODULE_TAGS := optional


LOCAL_CFLAGS := $(CFG_HI_CFLAGS) $(CFG_HI_BOARD_CONFIGS)
LOCAL_CFLAGS += -DLOG_TAG=\"$(LOCAL_MODULE)\"
#LOCAL_CFLAGS += -DDEBUG_LOG

#LOCAL_CFLAGS += -D宏名
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog

LOCAL_C_INCLUDES := $(COMMON_UNF_INCLUDE)
LOCAL_C_INCLUDES += $(COMMON_DRV_INCLUDE)
LOCAL_C_INCLUDES += $(COMMON_API_INCLUDE)
LOCAL_C_INCLUDES += $(MSP_UNF_INCLUDE)
LOCAL_C_INCLUDES += $(MSP_DRV_INCLUDE)
LOCAL_C_INCLUDES += $(MSP_API_INCLUDE)
LOCAL_C_INCLUDES += $(SAMPLE_DIR)/common
LOCAL_C_INCLUDES += $(COMPONENT_DIR)/ha_codec/include\


ifeq ("BUILD_EXECUTABLE",$(Makefile_Target))
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../export
else
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../include
endif
LOCAL_C_INCLUDES += $(LOCAL_PATH)

LOCAL_C_INCLUDES += $(ANDROID_BUILD_TOP)/ndk/sources/cxx-stl/stlport/stlport \

$(warning LOCAL_C_INCLUDES=$(LOCAL_C_INCLUDES))
#会生成依赖关系，当库不存在时会去编译这个库。
LOCAL_SHARED_LIBRARIES := libhi_common libc libcutils libutils libbinder libhi_msp libhi_sample_common libhidisplayclient libhidisplayservice liblog

#传递非系统的第三方库
#传递给ld的连接参数//大小端-EL{B}、编译器优化参数-O{}
#LOCAL_LDFLAGS += $(LOCAL_PATH)/lib/XX.a
#存在被依赖靠后的隐性要求
#LOCAL_LDFLAGS += $(LOCAL_PATH)/../lib/libXXX.a

ifeq ("BUILD_EXECUTABLE",$(Makefile_Target))
LOCAL_LDFLAGS += $(LOCAL_PATH)/../export/libcutest.a
endif

#编译Demo的可执行文件
ifeq ("BUILD_EXECUTABLE",$(Makefile_Target))
LOCAL_SRC_FILES += AllTests.c
LOCAL_SRC_FILES += CuTestTest.c
else
LOCAL_SRC_FILES += CuTest.c
endif
#BUILD_SHARED_LIBRARY			lib$(LOCAL_MODULE).so
#BUILD_STATIC_LIBRARY			lib$(LOCAL_MODULE).a
#BUILD_EXECUTABLE 				
ifeq ("BUILD_EXECUTABLE",$(Makefile_Target))
 	include $(BUILD_EXECUTABLE)
$(warning step1$(shell pwd))
$(warning step2$(shell mkdir "$(LOCAL_PATH)/../export"))
$(warning step3$(shell cp "$(ANDROID_BUILD_TOP)/out/target/product/$(CPU_TYPE)/system/bin/$(LOCAL_MODULE)" "$(LOCAL_PATH)/../export/" -rf))

else
	LOCAL_MODULE := lib$(LOCAL_MODULE)
	
	ifeq ("BUILD_SHARED_LIBRARY",$(Makefile_Target))
		include $(BUILD_SHARED_LIBRARY)
	else
		include $(BUILD_STATIC_LIBRARY)
	endif
	#当前聚焦目录为根目录
$(warning step1$(shell pwd))
$(warning step2$(shell mkdir "$(LOCAL_PATH)/../export/"))
$(warning step3$(shell cp "$(LOCAL_PATH)/../include/CuTest.h" "$(LOCAL_PATH)/../export/"))

ifeq ("BUILD_SHARED_LIBRARY",$(Makefile_Target))
$(warning step4$(shell cp "$(ANDROID_BUILD_TOP)/out/target/product/$(CPU_TYPE)/system/lib/$(LOCAL_MODULE).so" "$(LOCAL_PATH)/../lib/" -rf))
$(warning step5$(shell cp "$(LOCAL_PATH)/../lib/$(LOCAL_MODULE).so" "$(LOCAL_PATH)/../export/"))
else
$(warning step4$(shell cp "$(ANDROID_BUILD_TOP)/out/target/product/$(CPU_TYPE)/obj/STATIC_LIBRARIES/libcutest_intermediates/$(LOCAL_MODULE).a" "$(LOCAL_PATH)/../lib/" -rf))
$(warning step5$(shell cp "$(LOCAL_PATH)/../lib/$(LOCAL_MODULE).a" "$(LOCAL_PATH)/../export/"))
endif
endif

