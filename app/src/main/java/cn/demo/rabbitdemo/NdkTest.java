package cn.demo.rabbitdemo;

/**
 * Created by Administrator on 2018/6/28.
 */

public class NdkTest {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("rabbitmq");
    }

    public native int connect(String hostname, int port, String username, String password, String vhost);
    public native int createchannel();
    public native int publish(String message);
    public native void disconnect();
    public native byte[] basicconsume(int channel, String queue);

}
