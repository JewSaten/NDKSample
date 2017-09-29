#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <android/log.h>
#include "rijndael.h"
#include "rijndael.c"
//#include "io_jewsaten_ndksample_AESUtils.h"
using namespace std;

#define AES_KEY   "2017_app_sec_123"
#define KEYBITS 128
#define CLASS_PATH_NAME "io/jewsaten/ndksample/AESUtils"
#define LOG_TAG    "AES-JNI"
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


extern "C" {

jbyteArray stringTojbyteArray(JNIEnv* env, string data) {
	if (data == "") {
		return NULL;
	}
	int length = strlen(data.c_str());
	jbyteArray bytes = (env)->NewByteArray(length);
	(env)->SetByteArrayRegion(bytes, 0, length, (jbyte*) data.c_str());
	return bytes;
}

string jbyteArraytoString(JNIEnv* env, jbyteArray bytes) {
	if (bytes == NULL) {
		return NULL;
	}
	char* rtn = NULL;
	jsize alen = env->GetArrayLength(bytes);
	jbyte* ba = env->GetByteArrayElements(bytes, JNI_FALSE);
	if (alen > 0) {
		rtn = (char*) malloc(alen + 1);
		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	string stemp(rtn);
	free(rtn);
	return stemp;
}

string encrypt(string data ,string pass){
	string encryptData="";
	unsigned long rk[RKLENGTH(KEYBITS)];
	unsigned char key[KEYLENGTH(KEYBITS)];
	int keyLength=sizeof(key);
	memset(key,0,keyLength);
	memcpy(key,pass.c_str(),keyLength);

	int nrounds = rijndaelSetupEncrypt(rk, key, KEYBITS);
	int length=data.length();
	int newLength=length+(16-length%16);
	unsigned char srcBytes[newLength];
	memset(srcBytes,'\0',newLength);
	memcpy(srcBytes,data.c_str(),length);

	unsigned char plaintext[16];
	unsigned char ciphertext[16];
	for(int index=0;index<newLength;index+=16){
		for (int j=0;j<16;j++){
			plaintext[j]=srcBytes[index+j];
		}
	    rijndaelEncrypt(rk, nrounds, plaintext, ciphertext);
		for(int k=0;k<16;k++){
			encryptData+=ciphertext[k];
		}
	}
	return encryptData;
}

string decrypt(jbyte* data,int length,string pass){
	string decryptData="";
	unsigned long rk[RKLENGTH(KEYBITS)];
	unsigned char key[KEYLENGTH(KEYBITS)];
	int keyLength=sizeof(key);
	memset(key,0,keyLength);
	memcpy(key,pass.c_str(),keyLength);

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
			decryptData+=plaintext[k];
		}
	}
	return decryptData;
}

JNIEXPORT jbyteArray JNICALL JNI_ENCODE(JNIEnv *env, jclass clazz, jbyteArray content){
    string contentStr = jbyteArraytoString(env, content);
    LOGE("AESEncode1=%s",contentStr.c_str());
    string chip = encrypt(contentStr, AES_KEY);
    LOGE("AESEncode2=%s",chip.c_str());
    return stringTojbyteArray(env, chip);
}

JNIEXPORT jbyteArray JNICALL JNI_DECODE(JNIEnv *env, jclass clazz, jbyteArray content){
    jsize length = env->GetArrayLength(content);
    jbyte* data = env->GetByteArrayElements(content, 0);
    string result = decrypt(data, length, AES_KEY);
    LOGE("AESDecode=%s",result.c_str());
    return stringTojbyteArray(env, result);
}

static JNINativeMethod gMethods[] = {
		{"AESEncode", "([B)[B", (void*)JNI_ENCODE},
		{"AESDecode", "([B)[B", (void*)JNI_DECODE},
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
