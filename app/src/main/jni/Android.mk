LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := AES

LOCAL_SRC_FILES := rijndael.c app.cpp

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)