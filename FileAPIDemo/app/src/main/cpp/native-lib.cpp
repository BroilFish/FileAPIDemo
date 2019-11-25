#include <jni.h>
#include <iostream>
#include <string.h>
#include <stdio.h>


using namespace std;


#define DEBUG 1

#if DEBUG
#include <android/log.h>
#include <unistd.h>
#include <malloc.h>
#include <stdlib.h>

#define  D(x...)  __android_log_print(ANDROID_LOG_ERROR,"*****zsy",x)
#else
#  define  D(...)  do {} while (0)
#endif

#ifndef _Included_JniFuncMain
#define _Included_JniFuncMain

#ifdef __cplusplus
extern "C"
{
#endif

string normal_path = "/sdcard/a.txt";
string crypt_path = "/sdcard/a_encrypt.txt";

// 获取libnative-lib.so的地址
unsigned int GetLibAddr()

{

    const char *szName = "libnative-lib.so";

    int nPid = getpid();

    unsigned int nBase = 0;

    char buffer[1024] = {0};

    sprintf(buffer,"/proc/%d/maps",nPid);

    FILE *fp = fopen(buffer,"r");

    if (fp != NULL)

    {

        while(fgets(buffer,sizeof(buffer),fp))

        {

            if(strstr(buffer,szName))

            {

                //分割字符串 返回-之前内容

                char *temp;

                temp = strtok(buffer,"-");

                nBase = strtoul(temp,NULL,16);

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


static void crypt2(string normal_path, string crypt_path)
{
    D("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\");
    FILE *stream;
    char msg[]="this is a test";
    char buf[20];
    if((stream=fopen(normal_path.c_str(),"w+"))==NULL)
    {
        D("Can not open output file %d",stderr);
    }
    fwrite(msg,1,strlen(msg)+1,stream);

    fseek(stream,0,SEEK_SET);
    fread(buf,1,strlen(msg)+1,stream);
    D("crypt2:fread=%s",buf);

    fclose(stream);

}

void readFile(char *path) {
    char a[30];
    FILE *fp;
    if ((fp = fopen(path, "r")) == NULL) {
        D("*****zsy 读文件函数：文件打开失败！");
    }

    fseek(fp, 0L, SEEK_END);
    int len = ftell(fp);
    rewind(fp);

    if (fread(a, 1, len, fp) != len) {
        D("****zsy 读文件函数：读取失败！");
    } else
    {
        D("****zsy 读文件内容：%s", a);
    }

    fclose(fp);

}


void writeFile(char p[], char *path) {
    FILE *fp;
    if ((fp = fopen(path, "wb")) == NULL) {
        D("*****zsy 写文件 打开文件失败");
        //exit(0);
    }

    int a=fwrite(p, sizeof(char), strlen(p)+1, fp);
//    D("*****zsy a=%d",a);
//    D("*****zsy sizeof(p)/ sizeof(char)=%d",sizeof(p)/ sizeof(char));
//    D("*****zsy sizep=%d",sizeof(p));
//    D("*****zsy sizec=%d",sizeof(char));
//    D("*****zsy strlen=%d",strlen(p));
    if ( a < 0) {
        D("*****zsy 写文件 写入失败");
    } else
    {
        D("*****zsy 写文件  %s",p);
    }
    fclose(fp);
}



JNIEXPORT jint JNICALL
Java_com_example_fileapidemo_MainActivity_fileEncryptJNI(JNIEnv *env, jclass clazz) {
    // TODO: implement fileEncryptJNI()

    D("libnative-lib.so的地址 0x%x\r\n",GetLibAddr());

    // 加密文件
    //crypt(normal_path,crypt_path);
    crypt2(normal_path,crypt_path);


    return 1;

}


#ifdef __cplusplus
}
#endif
#endif

extern "C"
JNIEXPORT void JNICALL
Java_com_example_fileapidemo_MainActivity_fileReadJNI(JNIEnv *env, jclass clazz) {
    // TODO: implement fileReadJNI()

    readFile((char *)normal_path.c_str());

}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_fileapidemo_MainActivity_fileWriteJNI(JNIEnv *env, jclass clazz) {
    // TODO: implement fileWriteJNI()

    char p[]="this is write demo";
    D("*****zsy p.length:%d", sizeof(p));
    writeFile(p,(char *)normal_path.c_str());

}