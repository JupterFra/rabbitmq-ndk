/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class cn_demo_rabbitdemo_NdkTest */

#ifndef _Included_cn_demo_rabbitdemo_NdkTest
#define _Included_cn_demo_rabbitdemo_NdkTest
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     cn_demo_rabbitdemo_NdkTest
 * Method:    connect
 * Signature: (Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_cn_demo_rabbitdemo_NdkTest_connect
  (JNIEnv *, jobject, jstring, jint, jstring, jstring, jstring);

/*
 * Class:     cn_demo_rabbitdemo_NdkTest
 * Method:    createchannel
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_cn_demo_rabbitdemo_NdkTest_createchannel
  (JNIEnv *, jobject);

/*
 * Class:     cn_demo_rabbitdemo_NdkTest
 * Method:    publish
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_cn_demo_rabbitdemo_NdkTest_publish
  (JNIEnv *, jobject, jstring);

/*
 * Class:     cn_demo_rabbitdemo_NdkTest
 * Method:    disconnect
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_cn_demo_rabbitdemo_NdkTest_disconnect
  (JNIEnv *, jobject);

/*
 * Class:     cn_demo_rabbitdemo_NdkTest
 * Method:    basicconsume
 * Signature: (ILjava/lang/String;)[B
 */
JNIEXPORT jbyteArray JNICALL Java_cn_demo_rabbitdemo_NdkTest_basicconsume
  (JNIEnv *, jobject, jint, jstring);

#ifdef __cplusplus
}
#endif
#endif