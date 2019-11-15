#include <jni.h>
#include <iostream>
#include <string>
#include <stdio.h>



using namespace std;


#define DEBUG 1

#if DEBUG
#include <android/log.h>
#include <unistd.h>

#  define  D(x...)  __android_log_print(ANDROID_LOG_ERROR,"*****zsy",x)
#else
#  define  D(...)  do {} while (0)
#endif

#ifndef _Included_JniFuncMain
#define _Included_JniFuncMain

#ifdef __cplusplus
extern "C"
{
#endif

    // 获取libnative-lib.so的地址
unsigned int GetLibAddr()

{

    char *szName = "libnative-lib.so";

    int nPid = getpid();

    unsigned int nBase = 0;

    char buffer[1024] = {0};

    sprintf(buffer,"/proc/%d/maps",nPid);

    FILE *fp = fopen(buffer,"r");

    if (fp != nullptr)

    {

        while(fgets(buffer,sizeof(buffer),fp))

        {

            if(strstr(buffer,szName))

            {

                //分割字符串 返回-之前内容

                char *temp;

                temp = strtok(buffer,"-");

                nBase = strtoul(temp,nullptr,16);

                D("BASE IS 0x%x\r\n",nBase);

                break;
            }
        }

    }

    fclose(fp);

    return nBase;

}

static char* jstringToChar(JNIEnv* env, jstring jstr) {
    char *rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("GB2312");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte *ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0) {
        rtn = (char *) malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    return rtn;
}

static void crypt(string normal_path, string crypt_path)
{
    //打开文件
    //FILE *crypt_fp = fopen((const char *)crypt_path.c_str(), "w");
    FILE *normal_fp = fopen(normal_path.c_str(), "r");

    char buff[255];
    fscanf(normal_fp, "%s", buff);
    __android_log_print(ANDROID_LOG_ERROR,"*****zsy","fscanf:%s",buff);

    fgets(buff, 255, (FILE*)normal_fp);
    __android_log_print(ANDROID_LOG_ERROR,"*****zsy","fgets:%s",buff);

    fgets(buff, 255, (FILE*)normal_fp);
    __android_log_print(ANDROID_LOG_ERROR,"*****zsy","%s",buff);
    //fclose(crypt_fp);
    fclose(normal_fp);
}
static jstring charTojstring(JNIEnv* env, const char* pat) {
    //定义java String类 strClass
    jclass strClass = env->FindClass("java/lang/String");
    //获取String(byte[],String)的构造器,用于将本地byte[]数组转换为一个新String
    jmethodID ctorID = env->GetMethodID(strClass, "getBytes", "(Ljava/lang/String;)[B");
    //建立byte数组
    jbyteArray bytes = (env)->NewByteArray(strlen(pat));
    //将char* 转换为byte数组
    (env)->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*) pat);
    // 设置String, 保存语言类型,用于byte数组转换至String时的参数
    jstring encoding = (env)->NewStringUTF("UTF-8");
    //将byte数组转换为java String,并输出
    return (jstring) (env)->NewObject(strClass, ctorID, bytes, encoding);
}

JNIEXPORT jstring JNICALL
Java_com_example_fileapidemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


JNIEXPORT jint JNICALL
Java_com_example_fileapidemo_MainActivity_fileEncryptJNI(JNIEnv *env, jclass clazz) {
    // TODO: implement fileEncryptJNI()

    string normal_path = "/sdcard/a.txt";
    string crypt_path = "/sdcard/a_encrypt.txt";

    D("libnative-lib.so的地址 0x%x\r\n",GetLibAddr());

    // 加密文件
    crypt(normal_path,crypt_path);

    return 1;

}


#ifdef __cplusplus
}
#endif
#endif

