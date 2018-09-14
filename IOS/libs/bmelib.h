//
//  bmelib.h
//  bmelib
//
//  Created by Konstantin Gedalin on 28/08/2018.
//  
//

#import <Foundation/Foundation.h>

//
//  bmelib.h
//  bmelib
//
//  Created by Konstantin Gedalin on 28/08/2018.
//  Copyright © 2018 binah.ai. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface bmelib : NSObject


/*! Generates encoded signal
 
 +(NSData*)beepEncode:(NSString*) mode
 Frequency       :(float)frequency
 silenceLength   :(float)sil
 repetitions     :(NSInteger)rep
 AAC             :(NSString *)AAC
 gain            :(float)gain
 error           :(NSString **)err;
 
 * @param mode  input representing mode type (Morse\PSK....).
 * @param frequency  input carrier frequency (18000,19500...)
 * @param sil  input silence length in seconds
 * @param rep  input number of beeps in output array
 * @param AAC  input code to covert to beep, must be 4 symbols
 * @param gain input Gain of beeps
 * @param err  output error if set, if not the "SUCCESS" string return
 */

+(NSData*)beepEncode:(NSString*)     mode
    Frequency       :(float)frequency
    silenceLength   :(float)sil
    repetitions     :(NSInteger)rep
    AAC             :(NSString *)AAC
    gain            :(float)gain
    error           :(NSString **)err;



/*! Returns all possible encoding modes
 */
+(NSMutableArray*)allowedModes;
/*!  Returns all possible encoding frequencies
 */
+ (NSMutableArray *)allowedFrequencies;

@end




