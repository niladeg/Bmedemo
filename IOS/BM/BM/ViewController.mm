//
//  ViewController.m
//  BM
//
//  Created by Konstantin Gedalin on 02/09/2018.
//  Copyright © 2018 binah.ai. All rights reserved.
//
#import "bmelib.h"
#import <AVFoundation/AVFoundation.h>
#import "ViewController.h"
#define SCROLL_VIEW_HEIGHT 790
@interface ViewController ()
@property (nonatomic,strong)AVAudioPlayer *player;
@end

@implementation ViewController
@synthesize backgroundButton;
@synthesize scrollView;
@synthesize mModesPicker;
@synthesize pickerViewFrequency;
@synthesize sliderSpeakerVolume;
@synthesize textFieldMode;
@synthesize textFieldFrequency;
@synthesize textFieldSilenceLength;
@synthesize textFieldCodeValue1;
@synthesize textFieldRepetitions;
@synthesize textFieldBeepMarkVolume;
- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}


- (void)dealloc
{
}


#pragma mark - View lifecycle

- (void)viewDidLoad
{
    [super viewDidLoad];
    textFieldMode.delegate=self;
    [scrollView setContentSize:CGSizeMake([UIScreen mainScreen].bounds.size.width, SCROLL_VIEW_HEIGHT)];
    
    isFrequencyPickerOpen = NO;
    isNumOfPhasesPickerOpen = NO;
    scrollViewOffset = 0;
    
    CGRect frame = pickerViewFrequency.frame;
    frame.origin.y = [UIScreen mainScreen].bounds.size.height;
    pickerViewFrequency.frame = frame;
    isFrequencyPickerOpen = NO;
    pickerViewFrequency.delegate=self;;
    pickerViewFrequency.showsSelectionIndicator=YES;
    
    
    isNumOfPhasesPickerOpen = NO;
    frame = mModesPicker.frame;
    frame.origin.y = [UIScreen mainScreen].bounds.size.height;
    mModesPicker.frame = frame;
    ismModesPicker = NO;
    mModesPicker.delegate=self;;
    mModesPicker.showsSelectionIndicator=YES;
    
    [self.view sendSubviewToBack:backgroundButton];
    
    // initialization
    
    mode =@"Morse";
    textFieldMode.text = mode;
    
    
    frequency      =18000;
    textFieldFrequency.text = [NSString stringWithFormat:@"%fkHz", frequency];
    textFieldFrequency.delegate=self;
    silenceLength   =0.5;
    textFieldSilenceLength.text = [NSString stringWithFormat:@"%f", silenceLength];
    textFieldSilenceLength.delegate=self;
    
    textFieldCodeValue1.text = [NSString stringWithFormat:@"0102"];
    textFieldCodeValue1.delegate=self;
    
    codeRepetition      = @"40";
    textFieldRepetitions.text=codeRepetition;
    textFieldRepetitions.delegate=self;
    
    beepMarkVolume =0.1;
    textFieldBeepMarkVolume.text = [NSString stringWithFormat:@"%f", beepMarkVolume];
    textFieldBeepMarkVolume.delegate=self;

    speakerVolume = 0.5;
    [sliderSpeakerVolume setValue:speakerVolume];
    
    
}


#pragma mark - UITextFieldDelegate Methods

- (BOOL)textFieldShouldBeginEditing:(UITextField *)textField {
    BOOL shouldBeginEditing = YES;
    
    if ([textField isEqual:textFieldFrequency]) {
        shouldBeginEditing = NO;
        [self showPicker:pickerViewFrequency];
    }
    if ([textField isEqual:textFieldMode]) {
        shouldBeginEditing = NO;
        [self showPicker:mModesPicker];
    }
    else {
        [self.view bringSubviewToFront:backgroundButton];
        selectedTextField = textField;
    }
    
    return shouldBeginEditing;
}


- (void)textFieldDidBeginEditing:(UITextField *)textField {
    //216 – keyboard height
    if ([UIScreen mainScreen].bounds.size.height - (textField.frame.origin.y + textField.frame.size.height + scrollView.contentOffset.y) < 216) {
        scrollViewOffset = 216 - ([UIScreen mainScreen].bounds.size.height - (textField.frame.origin.y + textField.frame.size.height - scrollView.contentOffset.y));
        [scrollView setContentOffset:CGPointMake(0, scrollView.contentOffset.y + scrollViewOffset + 20) animated:YES];
    }
}


- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    BOOL shouldReturn = NO;
    
    if (![textField isEqual:textFieldFrequency] && [textField isEqual:textFieldMode]) {
        [textField resignFirstResponder];
        shouldReturn = YES;
        if (scrollViewOffset != 0) {
            [scrollView setContentOffset:CGPointMake(0, scrollView.contentOffset.y - scrollViewOffset - 20) animated:YES];
            scrollViewOffset = 0;
        }
        else if ([textField isEqual:textFieldSilenceLength])
            silenceLength = [textField.text intValue];
        else if ([textField isEqual:textFieldCodeValue1])
            codeValue1 = textField.text;
        else if ([textField isEqual:textFieldRepetitions])
            codeRepetition = textField.text;
        else if ([textField isEqual:textFieldBeepMarkVolume])
            beepMarkVolume = [textField.text floatValue];
        else if ([textField isEqual:textFieldMode])
            mode = textFieldMode.text;
        
        
        
    }
    
    return shouldReturn;
}


#pragma mark - IBActions

- (IBAction)backgroundButtonPressed:(id)sender {
    [self.view sendSubviewToBack:backgroundButton];
    if (scrollViewOffset != 0) {
        [scrollView setContentOffset:CGPointMake(0, scrollView.contentOffset.y - scrollViewOffset - 20) animated:YES];
        scrollViewOffset = 0;
    }
    if (isFrequencyPickerOpen)
        [self hidePicker:pickerViewFrequency];
   
    if (ismModesPicker)
        [self hidePicker:mModesPicker];
    [selectedTextField resignFirstResponder];
    if ([selectedTextField isEqual:textFieldSilenceLength])
        silenceLength = [selectedTextField.text intValue];
    else if ([selectedTextField isEqual:textFieldCodeValue1])
        codeValue1 = selectedTextField.text;
    else if ([selectedTextField isEqual:textFieldRepetitions])
        codeRepetition = selectedTextField.text;
    else if ([selectedTextField isEqual:textFieldBeepMarkVolume])
        beepMarkVolume = [selectedTextField.text floatValue];
    else if ([selectedTextField isEqual:textFieldMode])
        mode = selectedTextField.text;
    
}


- (IBAction)sliderValueChanged:(id)sender {
    speakerVolume = ((UISlider *)sender).value;
}

-(NSMutableData*)prepareforplay:(NSData*) Wave
{
    
    unsigned long totalAudioLen=[Wave length];
    unsigned long totalDataLen = totalAudioLen + 44;
    unsigned long longSampleRate = 44100;
    unsigned int channels = 1;
    unsigned long byteRate = (16 * longSampleRate * channels)/8;
    
    Byte *header = (Byte*)malloc(44);
    header[0] = 'R';  // RIFF/WAVE header
    header[1] = 'I';
    header[2] = 'F';
    header[3] = 'F';
    header[4] = (Byte) (totalDataLen & 0xff);
    header[5] = (Byte) ((totalDataLen >> 8) & 0xff);
    header[6] = (Byte) ((totalDataLen >> 16) & 0xff);
    header[7] = (Byte) ((totalDataLen >> 24) & 0xff);
    header[8] = 'W';
    header[9] = 'A';
    header[10] = 'V';
    header[11] = 'E';
    header[12] = 'f';  // 'fmt ' chunk
    header[13] = 'm';
    header[14] = 't';
    header[15] = ' ';
    header[16] = 16;  // 4 bytes: size of 'fmt ' chunk
    header[17] = 0;
    header[18] = 0;
    header[19] = 0;
    header[20] = 1;  // format = 1 for pcm and 2 for byte integer
    header[21] = 0;
    header[22] = (Byte) channels;
    header[23] = 0;
    header[24] = (Byte) (longSampleRate & 0xff);
    header[25] = (Byte) ((longSampleRate >> 8) & 0xff);
    header[26] = (Byte) ((longSampleRate >> 16) & 0xff);
    header[27] = (Byte) ((longSampleRate >> 24) & 0xff);
    header[28] = (Byte) (byteRate & 0xff);
    header[29] = (Byte) ((byteRate >> 8) & 0xff);
    header[30] = (Byte) ((byteRate >> 16) & 0xff);
    header[31] = (Byte) ((byteRate >> 24) & 0xff);
    header[32] = (Byte) (16*1)/8;  // block align
    header[33] = 0;
    header[34] = 16;  // bits per sample
    header[35] = 0;
    header[36] = 'd';
    header[37] = 'a';
    header[38] = 't';
    header[39] = 'a';
    header[40] = (Byte) (totalAudioLen & 0xff);
    header[41] = (Byte) ((totalAudioLen >> 8) & 0xff);
    header[42] = (Byte) ((totalAudioLen >> 16) & 0xff);
    header[43] = (Byte) ((totalAudioLen >> 24) & 0xff);
    
    NSData *headerData = [NSData dataWithBytes:header length:44];
    NSMutableData * soundFileData = [NSMutableData alloc];
    [soundFileData appendData:headerData];
    [soundFileData appendData:Wave];
    return soundFileData;
}

- (IBAction)goButtonPressed:(id)sender {
   
    NSString *err=[[NSString alloc] init];
    NSData *encodeddata=[bmelib beepEncode:textFieldMode.text
                                            Frequency:[textFieldFrequency.text floatValue]
                                            silenceLength   :[textFieldSilenceLength.text floatValue]
                                            repetitions     :[textFieldRepetitions.text integerValue]
                                            AAC             :textFieldCodeValue1.text
                                            gain            :[textFieldBeepMarkVolume.text floatValue]
                                            error           :&err ];
    
    if(encodeddata!=nil)
    {
    NSMutableData * soundFileData=[self prepareforplay: encodeddata];
     self.player = [[AVAudioPlayer alloc] initWithData:soundFileData fileTypeHint:@"wav" error:nil];
     float val= self.player.volume;
     self.player.volume = val*speakerVolume;
     self.player.numberOfLoops = 0; //Infinite
      [self.player play];
    }
    else
        NSLog(err);
    
}

-(void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag
{
    NSLog(@"%d",flag);
}

#pragma mark - Animation Methods

- (void)showPicker:(UIPickerView *)pickerView {
    [self.view bringSubviewToFront:backgroundButton];
    if ([pickerView isEqual:pickerViewFrequency]) {
        if (!isFrequencyPickerOpen) {
            isFrequencyPickerOpen = YES;
            [pickerView selectRow:[self selectedRowInPicker:pickerView] inComponent:0 animated:NO];
            [self.view bringSubviewToFront:pickerView];
            
            [UIView beginAnimations:nil context:nil];
            [UIView setAnimationDuration:0.3];
            
            CGRect frame = pickerView.frame;
            frame.origin.y -= frame.size.height;
            pickerView.frame = frame;
            
            [UIView commitAnimations];
        }
    }
    else if ([pickerView isEqual:mModesPicker])
    {
        
        if (!ismModesPicker) {
            ismModesPicker = YES;
            [pickerView selectRow:[self selectedRowInPicker:pickerView] inComponent:0 animated:NO];
            [self.view bringSubviewToFront:pickerView];
            
            [UIView beginAnimations:nil context:nil];
            [UIView setAnimationDuration:0.3];
            
            CGRect frame = pickerView.frame;
            frame.origin.y -= frame.size.height;
            pickerView.frame = frame;
            
            [UIView commitAnimations];}
    }
    
}


- (void)hidePicker:(UIPickerView *)pickerView {
    [self.view sendSubviewToBack:backgroundButton];
    if ([pickerView isEqual:pickerViewFrequency]) {
        if (isFrequencyPickerOpen) {
            isFrequencyPickerOpen = NO;
            frequency = [((NSNumber *)[[bmelib allowedFrequencies] objectAtIndex:[pickerView selectedRowInComponent:0]]) intValue];
            textFieldFrequency.text = [NSString stringWithFormat:@"%f", frequency];
            
            [UIView beginAnimations:nil context:nil];
            [UIView setAnimationDuration:0.3];
            
            CGRect frame = pickerView.frame;
            frame.origin.y = [UIScreen mainScreen].bounds.size.height;
            pickerView.frame = frame;
            
            [UIView commitAnimations];
            [self.view sendSubviewToBack:pickerView];
        }
    }else  if ([pickerView isEqual:mModesPicker]) {
        if (ismModesPicker) {
            ismModesPicker = NO;
            mode = [[bmelib allowedModes] objectAtIndex:[pickerView selectedRowInComponent:0]];
            textFieldMode.text = mode;
            
            [UIView beginAnimations:nil context:nil];
            [UIView setAnimationDuration:0.3];
            
            CGRect frame = pickerView.frame;
            frame.origin.y = [UIScreen mainScreen].bounds.size.height;
            pickerView.frame = frame;
            
            [UIView commitAnimations];
            [self.view sendSubviewToBack:pickerView];
        }
    }
    
    
}


#pragma mark - UIPickerViewDataSource Methods

- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView {
    return 1;
}


- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component {
    NSInteger numberOfRows = 0;
    
    if ([pickerView isEqual:pickerViewFrequency])
        numberOfRows = [[bmelib allowedFrequencies] count];
    else if ([pickerView isEqual:mModesPicker])
        numberOfRows = [[bmelib allowedModes] count];
    
    
    return numberOfRows;
}


#pragma mark - UIPickeViewDelegate Methods

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component {
    NSString *title;
    
    if ([pickerView isEqual:pickerViewFrequency])
        title = [NSString stringWithFormat:@"%f", [((NSNumber *)[[bmelib allowedFrequencies] objectAtIndex:row]) floatValue]];
    else if ([pickerView isEqual:mModesPicker])
        title = [[bmelib allowedModes] objectAtIndex:row];
    
    
    return title;
}


- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component {
    [self hidePicker:pickerView];
}


#pragma mark - Picker Custom Methods

- (NSInteger)selectedRowInPicker:(UIPickerView *)pickerView {
    NSInteger selectedRow = 0;
    
    if ([pickerView isEqual:pickerViewFrequency]) {
        for (int i = 0; i < [[bmelib allowedFrequencies] count]; i++) {
            if ([((NSNumber *)[[bmelib allowedFrequencies] objectAtIndex:i]) intValue] == frequency) {
                selectedRow = i;
                break;
            }
        }
    }
    
    else if ([pickerView isEqual:mModesPicker]) {
        for (int i = 0; i < [[bmelib allowedModes] count]; i++) {
            if ([[[bmelib allowedModes] objectAtIndex:i] isEqual:mode]) {
                selectedRow = i;
                break;
            }
        }
    }
    
    
    return selectedRow;
}


#pragma mark - bmelibDelegate Methods

- (void)bmelibFinishedPlaying {
    NSLog(@"Finished playing sound.");
}


- (void)bmelibFailedWithError:(NSString *)errorDescription {
    UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:@"BME SDK ERROR"
                                                        message:errorDescription
                                                       delegate:self
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
    [alertView show];
   // [alertView release];
}


- (void)bmelibLogEntry:(NSString *)logLine {
    NSLog(@"Log: %@", logLine);
}

-(NSString *) stringToHex:(NSString *)str withDelimiter:(int)delim
{
    char a=' ';
    NSUInteger len = [str length];
    unichar *chars = (unichar*)malloc(len * sizeof(unichar));
    [str getCharacters:chars];
    
    NSMutableString *hexString = [[NSMutableString alloc] init];
    int n=len%delim;
    for(NSUInteger i = 0; i < len; i++ )
    {
        // [hexString [NSString stringWithFormat:@"%02x", chars[i]]]; /*previous input*/
        if (i!=0 && !(i%delim))
            [hexString appendFormat:@";"];
        [hexString appendFormat:@"%02X", chars[i]]; /*EDITED PER COMMENT BELOW*/
    }
    free(chars);
    
    
    if (n)
    {
        for(int i=0;i<delim-n;++i)
            [hexString appendFormat:@"%02X", a];
    }
    return hexString;
}

@end
