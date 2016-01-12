#��ʾ"��ǰĿ¼��û����Ҫ������ļ���������Ŀ¼����"��
#�Ǹ��߱�����������Ŀ¼��ݹ��һ�ֹ涨д��
#�����굱ǰĿ¼��Makefile�������ٱ�����Ŀ¼��Makefile
#��ǰĿ¼û��Makefile, �������Ŀ¼�е�Makefile
#include $(call all-named-subdir-makefiles,$(AO_SAMPLE_MODULE))

#��ʾ"��ǰĿ¼��û����Ҫ������ļ���������Ŀ¼����"��
#�Ǹ��߱�����������Ŀ¼��ݹ��һ�ֹ涨д��
#�����굱ǰĿ¼��Makefile�����ٱ�����Ŀ¼��Makefile
#���ܵ�ǰĿ¼û��Makefile, �������ָ������Ŀ¼�е�Makefile
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

#LOCAL_CFLAGS += -D����
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
#������������ϵ�����ⲻ����ʱ��ȥ��������⡣
LOCAL_SHARED_LIBRARIES := libhi_common libc libcutils libutils libbinder libhi_msp libhi_sample_common libhidisplayclient libhidisplayservice liblog

#���ݷ�ϵͳ�ĵ�������
#���ݸ�ld�����Ӳ���//��С��-EL{B}���������Ż�����-O{}
#LOCAL_LDFLAGS += $(LOCAL_PATH)/lib/XX.a
#���ڱ��������������Ҫ��
#LOCAL_LDFLAGS += $(LOCAL_PATH)/../lib/libXXX.a

ifeq ("BUILD_EXECUTABLE",$(Makefile_Target))
LOCAL_LDFLAGS += $(LOCAL_PATH)/../export/libcutest.a
endif

#����Demo�Ŀ�ִ���ļ�
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
	#��ǰ�۽�Ŀ¼Ϊ��Ŀ¼
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

