package com.scifold.beep.bmedemo;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;



import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import android.app.Activity;
import android.media.AudioTrack;
import android.media.AudioManager;
import android.media.AudioFormat;

import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.Spinner;
import android.widget.Toast;

import com.beeple.bmesdk.BmeSdk;

public class MainActivity extends Activity {

    AudioTrack audioTrack;
    private Boolean mStartButtonStatus = false;
    private Spinner mFrequency;

    private Spinner mMModes;

    private EditText mRepetitions;

    private EditText mSilenceLength;

    private EditText mCodeValues;
    private EditText mBeepVolume;

    private SeekBar mSpeakerVolume;

    private Button mGo;


    private List<String> mFreqArray;

    private List<String> mNumOfModesArray;

    public BmeSdk mBmeSdk;

    // BME-SDK-PSK
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        mFrequency = (Spinner) findViewById(R.id.spinnerfrc);
        mMModes = (Spinner) findViewById(R.id.spinnerModes);

        mRepetitions = (EditText) findViewById(R.id.mRep);
        mSilenceLength = (EditText) findViewById(R.id.mSil);
        mCodeValues = (EditText) findViewById(R.id.mAAC);
        mBeepVolume = (EditText) findViewById(R.id.mGain);
        mGo = (Button) findViewById(R.id.mStart);
        mBmeSdk = new BmeSdk();
        // **********************************************************

         populateFreqSpinner();
         populateMModesSpinner();
        Map<String, String> defaultDictionary = BmeSdk.defaultValues();


       mFrequency.setSelection(mFreqArray.indexOf(defaultDictionary.get("Frequency")));
       mMModes.setSelection(mNumOfModesArray.indexOf("Morse"));
       mSilenceLength.setText(defaultDictionary.get("Silence Length"));
       mBeepVolume.setText(defaultDictionary.get("Beep mark Volume"));
      //  mSpeakerVolume
      //          .setProgress((int) (Float.parseFloat(defaultDictionary.get("Speaker Volume")) * 10));

        // Defaults values to prevent application from crashing
        // important for tests only
        mCodeValues.setText("0102");
        mRepetitions.setText("40");
        mBeepVolume.setText("0.5");

        //Default wav file located in assets

        mGo.setOnClickListener(mGoListener);





    }
    public void play(int sampleRate, int channelConfiguration, int audioFormat, short [] buffer) {
        int minBufferSize =AudioTrack.getMinBufferSize(sampleRate, channelConfiguration, audioFormat);
        audioTrack = new AudioTrack(AudioManager.STREAM_MUSIC, sampleRate, channelConfiguration,
                audioFormat, minBufferSize, AudioTrack.MODE_STREAM);

        audioTrack.setStereoVolume((float)1.0, (float)1.0);
        int r=buffer.length/minBufferSize;
        audioTrack.play();
        audioTrack.write(buffer, 0, buffer.length);

    }


    OnClickListener mGoListener = new OnClickListener() {

        public void onClick(View v) {
            if (mStartButtonStatus == false) {


                try {
                    mStartButtonStatus = true;
                    mGo.setText("Stop");
                    // Second parameter for method initWithMode changed from String to short []
                    //  Example usage of second parameter - recorded raw data for mixing with Beep signal
                    // retrieve short data from wav file (pcm 16 bit format) located in assets
                    String sfrq =  mFrequency.getSelectedItem().toString();
                    float frequency= Float.parseFloat(sfrq);
                    String ssilenceLength = mSilenceLength.getText().toString();
                    float silenceLength= Float.parseFloat(ssilenceLength);
                    String sbeepMarkVolume =  mBeepVolume.getText().toString();
                    float beepMarkVolume= Float.parseFloat(sbeepMarkVolume);
                    String sCodeRepetitions =  mRepetitions.getText().toString();
                    int CodeRepetitions= Integer.parseInt(sCodeRepetitions);

                    String AInit=mBmeSdk.init(mMModes.getSelectedItem().toString(),frequency, silenceLength, mCodeValues.getText().toString(),beepMarkVolume,CodeRepetitions);
                    if(AInit.equalsIgnoreCase("SUCCEED"))
                    {
                      short [] res= mBmeSdk.getSound();
                      play(44100, AudioFormat.CHANNEL_OUT_MONO , AudioFormat.ENCODING_PCM_16BIT, res);


                    }




                } catch (NumberFormatException e) {
                    int duration = Toast.LENGTH_SHORT;
                    Toast toast = Toast.makeText(getApplicationContext(), "Illegal value found.",
                            duration);

                    toast.show();

                    mStartButtonStatus = false;
                    mGo.setText("Start");
                }

            } else {
                mStartButtonStatus = false;
                mGo.setText("Go");
            }

        }
    };

    public byte [] ReadByteArrayFromFile(String name)
    {

        try{
            InputStream is=getAssets().open(name);
            byte[] fileBytes=new byte[is.available()];
            is.read( fileBytes);
            is.close();
            return fileBytes;
        }catch (Exception e) {
            e.getMessage();
        }
        byte[] fileBytes=new byte[1];
        return fileBytes;
    }



    private void populateFreqSpinner() {
        mFreqArray = new ArrayList<String>();
        for (Float i : BmeSdk.allowedFrequencies()) {
            mFreqArray.add(i.toString());
        }
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_spinner_item, mFreqArray);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mFrequency.setAdapter(adapter);
    }

    private void populateMModesSpinner() {
        mNumOfModesArray = new ArrayList<String>();
        List<String> v=BmeSdk.allowedModes();
        for (int i=0;i<v.size();i++ ) {
            mNumOfModesArray.add(v.get(i));
        }
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_spinner_item, mNumOfModesArray);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mMModes.setAdapter(adapter);
    }
}
