//
//  ViewController.h
//  BM
//
//  Created by Konstantin Gedalin on 02/09/2018.
//  Copyright © 2018 binah.ai. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController<UITextFieldDelegate, UIPickerViewDelegate>
{
NSString    *mode;
NSString    *codeValue1;
NSString    *codeRepetition;


float   frequency;
float   silenceLength;
float   beepMarkVolume;
float   speakerVolume;
int     repetitions;
float   gain;
NSString *AAC;
    
BOOL isFrequencyPickerOpen;
BOOL isNumOfPhasesPickerOpen;
BOOL ismModesPicker;
NSInteger scrollViewOffset;
UITextField             *selectedTextField;
}
@property (weak, nonatomic) IBOutlet UIButton *backgroundButton;
@property (nonatomic, retain) IBOutlet UIScrollView *scrollView;
@property (weak, nonatomic) IBOutlet UIPickerView *mModesPicker;
@property (weak, nonatomic) IBOutlet UIPickerView *pickerViewFrequency;
@property (weak, nonatomic) IBOutlet UISlider *sliderSpeakerVolume;
@property (weak, nonatomic) IBOutlet UITextField *textFieldFrequency;
@property (weak, nonatomic) IBOutlet UITextField *textFieldSilenceLength;
@property (weak, nonatomic) IBOutlet UITextField *textFieldCodeValue1;
@property (weak, nonatomic) IBOutlet UITextField *textFieldRepetitions;
@property (weak, nonatomic) IBOutlet UITextField *textFieldBeepMarkVolume;
@property (weak, nonatomic) IBOutlet UITextField *textFieldMode;

#pragma mark - IBActions
- (IBAction)backgroundButtonPressed:(id)sender;
- (IBAction)sliderValueChanged:(id)sender;
- (IBAction)goButtonPressed:(id)sender;

#pragma mark - Animation Methods
- (void)showPicker:(UIPickerView *)pickerView;
- (void)hidePicker:(UIPickerView *)pickerView;

#pragma mark - Picker Custom Methods
- (NSInteger)selectedRowInPicker:(UIPickerView *)pickerView;
-(NSMutableData*)prepareforplay:(NSData*) wavedata;


@end

