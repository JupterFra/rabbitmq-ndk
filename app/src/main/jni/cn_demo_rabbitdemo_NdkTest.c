#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include <amqp.h>
#include <amqp_tcp_socket.h>
#include <utils.h>

#include <assert.h>
#include "cn_demo_rabbitdemo_NdkTest.h"
#include <android/log.h>
#include <sys/time.h>

#define LOG_TAG    "NdkTest.c"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGD(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

const char *TAG = "rabbitmq_test:JNI:ndktests.c";
amqp_connection_state_t g_conn;
int g_sockfd;
int g_channel = 0;
jbyteArray jbytes_cache;
unsigned char *buf;
int BUFFER_SIZE = 1024;


/*begin java interface functions*/
jint JNICALL Java_cn_demo_rabbitdemo_NdkTest_connect
  (JNIEnv *env, jobject obj, jstring jhostname, jint port, jstring jusername, jstring jpassword, jstring jvhost)
{
    const char *hostname;
	const char *username;
	const char *password;
    const char *vhost;


	hostname = (*env)->GetStringUTFChars(env, jhostname, 0);
	username = (*env)->GetStringUTFChars(env, jusername, 0);
	password = (*env)->GetStringUTFChars(env, jpassword, 0);
    vhost = (*env)->GetStringUTFChars(env, jvhost, 0);

	LOGE("======CONNECTING TO MQ SERVER======");

	int sockfd;

    LOGE("======Initial amqp connection======");
    g_conn = amqp_new_connection();

    die_on_error(sockfd = amqp_open_socket(hostname, port), "Opening socket");

    amqp_set_sockfd(g_conn, sockfd);

    LOGE("======amqp login======");
    die_on_amqp_error(amqp_login(g_conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, username, password), "login channel");

    g_sockfd = sockfd;

	return 1;
}

jint JNICALL Java_cn_demo_rabbitdemo_NdkTest_createchannel
  (JNIEnv * env, jobject obj)
{
    int channel = 1;
	amqp_channel_open(g_conn, channel);
	LOGE("======channel opened======");
	die_on_amqp_error(amqp_get_rpc_reply(g_conn), "get rpc reply");
	return channel;
}

jint JNICALL Java_cn_demo_rabbitdemo_NdkTest_publish
  (JNIEnv * env, jobject obj, jstring message){
        amqp_basic_properties_t props;
        LOGE("message : %s", message);
        const char* str;
        str = (*env)->GetStringUTFChars(env,message,NULL);
        props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
        props.content_type = amqp_cstring_bytes("text/plain");
        props.delivery_mode = 2; /* persistent delivery mode */
        die_on_error(amqp_basic_publish(g_conn,
                                    1,
                                    amqp_cstring_bytes("amq.fanout"),
                                    amqp_cstring_bytes(""),
                                    0,
                                    0,
                                    &props,
                                    amqp_cstring_bytes(str)),
                 "Publishing");
}

void JNICALL Java_cn_demo_rabbitdemo_NdkTest_disconnect
  (JNIEnv *env, jobject obj)
{
    amqp_connection_close(g_conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(g_conn);
    close(g_sockfd);
    LOGE("DISCONNECTED FROM MQ SERVER");
}

jbyteArray JNICALL Java_cn_demo_rabbitdemo_NdkTest_basicconsume
  (JNIEnv *env, jobject obj, jint channel, jstring jqueue_name)
{

        amqp_bytes_t queuename;

        {
            amqp_queue_declare_ok_t *r = amqp_queue_declare(g_conn, 1, amqp_empty_bytes, 0, 0, 0, 1, amqp_empty_table);
            die_on_amqp_error(amqp_get_rpc_reply(g_conn), "Declaring queue");

            queuename = amqp_bytes_malloc_dup(r->queue);
        }

        if (queuename.bytes == NULL)
        {
            LOGE("Out of memory while copying queue name");
        }

        amqp_queue_bind(g_conn, 1, queuename, amqp_cstring_bytes("amq.fanout"),
                        amqp_cstring_bytes(""), amqp_empty_table);
        die_on_amqp_error(amqp_get_rpc_reply(g_conn), "Binding queue");

       //like the rabbitmq-c/examples/amqp_listenq.c
        amqp_basic_consume(g_conn, 1, queuename, AMQP_EMPTY_BYTES, 0, 0, 0,amqp_empty_table);
        die_on_amqp_error(amqp_get_rpc_reply(g_conn), "Consuming");

        {
            amqp_rpc_reply_t res;
            amqp_envelope_t envelope;
            amqp_maybe_release_buffers(g_conn);
            struct timeval tval;
            struct timeval *tv;
            tv=&tval;

            tv->tv_sec=0.1;
            tv->tv_usec=0;
            res = amqp_consume_message(g_conn, &envelope,NULL, 0);
            if (AMQP_RESPONSE_NORMAL != res.reply_type)
            {
                return 0;
            }
            buf = envelope.message.body.bytes;
            BUFFER_SIZE = envelope.message.body.len;
            amqp_dump(envelope.message.body.bytes, envelope.message.body.len);

        }
        jbytes_cache = (*env)->NewByteArray(env, BUFFER_SIZE);

        (*env)->SetByteArrayRegion(env, jbytes_cache, 0, BUFFER_SIZE, buf);
         return jbytes_cache;
}