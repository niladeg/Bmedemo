package com.beeple.bmesdk;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import static java.util.Arrays.asList;





public class BmeSdk {

    public static final String  ERROR_SILENCE = "ERROR! silence length is illegal!";
    public static final String  ERROR_FEREQUENCY = "ERROR! frequency is illegal!";
    public static final String  ERROR_MODE   ="ERROR! MODE ILLEGAL!" ;
    public static final String  ERROR_GAIN   ="ERROR! Background File format error!" ;
    public static final String  ERROR_CODE   ="ERROR! Message code wrong!" ;
    public static final String  ERROR_REPT  ="ERROR! Number repetitions wrong!" ;
    public static final String  SUCCEED  ="SUCCEED" ;

    private static final String TAG = "BME PskVer 2:1:27";
    private static float [] WorkingFrequencies={1800,4000,8000,10000,12000,14000,15000,17000,18000,18500,19000,19500};
    private static List<String>  AllowedModes=asList("PSK","Morse","FDM","FDMA","OFDM","DPSK");
	public  BmeSdk(){};
	String _mode;
	float _frequency;
	float _silenceLength;
	String _codeValue1;
	float _beepMarkVolume;
	int _CodeRepetitions;


	/*! Initializes encoding and returns error string , on succeed return "SUCCEED"

		 public String init(	String mode,
							  	float frequency,
								float silenceLength,
								String codeValue1,
								float beepMarkVolume,
								int CodeRepetitions)

 * @param mode  input representing mode type (Morse\PSK....).
 * @param frequency  input carrier frequency (18000,19500...)
 * @param silenceLength  input silence length in seconds
 * @param CodeRepetitions  input number of beeps in output array
 * @param codeValue1  input code to covert to beep, must be 4 symbols
 * @param beepMarkVolume input Gain of beeps

 */


	public String init(	  	String mode,
							  	float frequency,
								float silenceLength,
								String codeValue1,
								float beepMarkVolume,
								int CodeRepetitions)
	 {

		    Boolean iModeLegal = false;
	        for (int i = 0; i < AllowedModes.size(); i++) {
	        	String val=AllowedModes.get(i);
	        	int res=val.compareToIgnoreCase(mode);
	            if (res==0) {
	            	iModeLegal = true;
	            	_mode=mode;
	                break;
	            }
	        }
	        
	        if (!iModeLegal){
	        	return ERROR_MODE;
	        }

	        Boolean isFrequencyLegal = false;
	        for (int i = 0; i < WorkingFrequencies.length; i++) {
	            if ( WorkingFrequencies[i] == frequency) {
	                isFrequencyLegal = true;
	                _frequency=frequency;
	                break;
	            }
	        }
	        
	        if (!isFrequencyLegal) {
	        	return ERROR_FEREQUENCY;
	        }

	        if (silenceLength < 0) {
	        	return ERROR_SILENCE;
	        }
	        _silenceLength=silenceLength;
	        if(CodeRepetitions<=0)
	        {
	        	return ERROR_REPT;
	        }
	        _CodeRepetitions=CodeRepetitions;

	        _codeValue1=codeValue1;
	        if(beepMarkVolume<0)
	        return ERROR_GAIN;
           _beepMarkVolume=beepMarkVolume;
           return SUCCEED;

	 }
    /*!Generates encoded signal, return short array of data

     public short [] getSound()

     */
    public short [] getSound()
    {
        short [] res =generateSound(_mode.getBytes(),_frequency,_codeValue1.getBytes(),_silenceLength,_beepMarkVolume,_CodeRepetitions);

        return res;

    }
	
	/*! Returns the default SDK values which are used if no value for some parameter is provided  */
	public static Map<String, String> defaultValues()
	{
	Map<String, String> dictionary = new HashMap<String, String>();
	float spkval=0.5f;
	String speakerdefaultvalue = Float.toString(spkval);
	dictionary.put("Gain", "0.3");
	dictionary.put("Speaker Volume", speakerdefaultvalue);
	dictionary.put("Silence Length", "1.0");
	dictionary.put("Mode", "Morse");
	dictionary.put("Frequency", "18000.0");
	return dictionary;
	}
  
	/*! Returns the frequencies supported by the SDK    */
	public static List<Float> allowedFrequencies()
	{

	    List<Float> intList = new ArrayList<Float>();
	    for (int index = 0; index < WorkingFrequencies.length; index++)
	    {
		        intList.add(WorkingFrequencies[index]);
	    }
	    return intList;
	}
    /*! Returns the modes supported by the SDK    */
    public static List<String> allowedModes()
    {
        return AllowedModes;
    }


	public static native short [] generateSound(byte [] mode,float frequency,byte [] code,float silenceLn,float gain,int repetitions);

    static {

        System.loadLibrary("bmelib");
    }

}
