package cn.demo.rabbitdemo;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class MainActivity extends AppCompatActivity {


    private String msg = "default";
    private String TAG = "MAIN_ACTIVITY";

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        final EditText etMsg;
        Button btnSend;
        Button btnGet;

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        etMsg =(EditText)findViewById(R.id.etMsg);
        btnGet=(Button) findViewById(R.id.btnGet);
        btnSend=(Button)findViewById(R.id.btnSend);

        btnSend.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(!etMsg.getText().toString().equals("")){
                    msg = etMsg.getText().toString();
                }
                new Thread(mPublish).run();
            }
        });

        btnGet.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new Thread(mConsume).run();
            }
        });

    }

    private Runnable mPublish = new Runnable() {
        @Override
        public void run() {
            NdkTest ndkTest = new NdkTest();
            Log.v(TAG,"send" + msg + "to rabbitMQ service");
            ndkTest.connect("192.168.1.103", 5672, "admin", "admin", "/");
            ndkTest.createchannel();
            ndkTest.publish(msg);
            ndkTest.disconnect();
        }
    };

    private Runnable mConsume = new Runnable() {
        @Override
        public void run() {
            while (true){
                NdkTest ndkTest = new NdkTest();
                ndkTest.connect("192.168.1.103", 5672, "admin", "admin", "/");
                ndkTest.createchannel();
                if(ndkTest.basicconsume(1,"ads")!=null){
                    String receiceMsg = new String(ndkTest.basicconsume(1,"ads")) ;
                    Log.v(TAG,"receive "+receiceMsg);
                }
                ndkTest.disconnect();
            }
        }
    };

}
