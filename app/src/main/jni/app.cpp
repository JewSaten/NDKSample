#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rijndael.h"
#include "rijndael.c"
//#include "io_jewsaten_ndksample_AESUtils.h"

#define AES_KEY   "2017_app_sec_123"
#define KEYBITS 128
#define CLASS_PATH_NAME "io/jewsaten/ndksample/AESUtils"
#define CLASS io_jewsaten_ndksample_AESUtils
#define JNI_FUNC(FUN) Java_##CLASS##_##FUN


extern "C" {

char*  Jstring2CStr(JNIEnv* env, jstring jstr){
     char* rtn = NULL;
     jclass   clsstring = env->FindClass("java/lang/String");
     jstring   strencode = env->NewStringUTF("utf-8");
     jmethodID   mid = env->GetMethodID(clsstring,   "getBytes",   "(Ljava/lang/String;)[B");
     jbyteArray   barr = (jbyteArray)env->CallObjectMethod(jstr,mid,strencode);
     jsize   alen = env->GetArrayLength(barr);
     jbyte*   ba = env->GetByteArrayElements(barr,JNI_FALSE);
     if(alen > 0){
        rtn = (char*)malloc(alen+1);
        memcpy(rtn,ba,alen);
        rtn[alen]=0;
     }
    env->ReleaseByteArrayElements(barr,ba,0);
    return rtn;
}


char* encrypt(char* data , const char* pass){
	char* encryptData;
	unsigned long rk[RKLENGTH(KEYBITS)];
	unsigned char key[KEYLENGTH(KEYBITS)];
	int keyLength=sizeof(key);
	memset(key,0,keyLength);
	memcpy(key,pass,keyLength);

	int nrounds = rijndaelSetupEncrypt(rk, key, KEYBITS);
	int length=strlen(data);
	int newLength=length+(16-length%16);
	unsigned char srcBytes[newLength];
	memset(srcBytes,'\0',newLength);
	memcpy(srcBytes,data,length);

	unsigned char plaintext[16];
	unsigned char ciphertext[16];
	for(int index=0;index<newLength;index+=16){
		for (int j=0;j<16;j++){
			plaintext[j]=srcBytes[index+j];
		}
	    rijndaelEncrypt(rk, nrounds, plaintext, ciphertext);
		for(int k=0;k<16;k++){
			encryptData = strcat(encryptData, (char*)ciphertext[k]);
		}
	}
	return encryptData;
}

char* decrypt(jbyte* data, int length, const char* pass){
	char* decryptData;
	unsigned long rk[RKLENGTH(KEYBITS)];
	unsigned char key[KEYLENGTH(KEYBITS)];
	int keyLength=sizeof(key);
	memset(key,0,keyLength);
	memcpy(key,pass,keyLength);

	int nrounds = rijndaelSetupDecrypt(rk, key, KEYBITS);
	unsigned char srcBytes[length];
	memset(srcBytes,0,length);
	memcpy(srcBytes,data,length);

	unsigned char plaintext[16];
	unsigned char ciphertext[16];
	for(int index=0;index<length;index+=16){
		for (int j=0;j<16;j++){
			ciphertext[j]=srcBytes[index+j];
		}
		rijndaelDecrypt(rk, nrounds, ciphertext, plaintext);
		for(int k=0;k<16;k++){
			decryptData = strcat(decryptData, (char*)plaintext[k]);
		}
	}
	return decryptData;
}



JNIEXPORT jstring JNICALL a(JNIEnv *env, jclass clazz, jstring content){
	char* src=Jstring2CStr(env,content);
	char* result=encrypt(src, AES_KEY);
	return env->NewStringUTF(result);
}

JNIEXPORT jstring JNICALL b(JNIEnv *env, jclass clazz, jbyteArray content){
	jsize length = env->GetArrayLength(content);
	jbyte* data = env->GetByteArrayElements(content, 0);
	char* result=decrypt(data, length, AES_KEY);
	return env->NewStringUTF(result);
}

static JNINativeMethod gMethods[] = {
		{"AESEncode", "(Ljava/lang/String;)Ljava/lang/String;", (void*)a},
		{"AESDecode", "([B)Ljava/lang/String;", (void*)b},
};

static int registerNativeMethods(JNIEnv* env, const char* className,
		JNINativeMethod* gMethods, int numMethods) {
	jclass clazz;
	clazz = env->FindClass(className);
	if (clazz == NULL) {
		return JNI_FALSE;
	}
	if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
		env->DeleteLocalRef(clazz);
		return JNI_FALSE;
	}
	env->DeleteLocalRef(clazz);
	return JNI_TRUE;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
	JNIEnv *env;
	if (vm->GetEnv((void **) &env, JNI_VERSION_1_6)) {
		return JNI_ERR;
	}
	if (registerNativeMethods(env, CLASS_PATH_NAME, gMethods,
			sizeof(gMethods) / sizeof(gMethods[0])) != JNI_TRUE) {
		return JNI_ERR;
	}
	return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved) {
	JNIEnv *env;
	jclass cls;
	if (vm->GetEnv((void **) &env, JNI_VERSION_1_6)) {
		return;
	}
	cls = env->FindClass(CLASS_PATH_NAME);
	env->UnregisterNatives(cls);
	return;
}

}
