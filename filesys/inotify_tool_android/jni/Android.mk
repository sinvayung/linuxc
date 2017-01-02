LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := inotify_tool
LOCAL_SRC_FILES := inotify_tool.c\
	FdNode.c\
	stat_ext.c
LOCAL_CFLAGS += -fPIE
LOCAL_LDFLAGS += -fPIE -pie

include $(BUILD_EXECUTABLE)