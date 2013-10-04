
#include "AudioAdapter.h"
#include "Logger.h"
#include "ApplicationProperty.h"

AudioAdapter::AudioAdapter(){
	stream=NULL;
	devCount=0;
	devinfo=NULL;
	memset( &inputParameters , 0 , sizeof( inputParameters ) );
	memset( &outputParameters , 0 , sizeof( outputParameters ) );
	inChan=0;
	outChan=0;
	inDevNum=0;
	outDevNum=0;
	framesPerBuffer=0;
	
	f_init=false;
	f_open=false;
	f_start=false;

	myInstance=NULL;

	phase=0.0f;
}

AudioAdapter::~AudioAdapter(){
}

AudioAdapter* AudioAdapter::myInstance=NULL;

AudioAdapter* AudioAdapter::CreateInstance(){
	if(myInstance==NULL){
		myInstance=new AudioAdapter;
	}
	return myInstance;
}
void AudioAdapter::DestroyInstance(){
	SAFE_DELETE(myInstance);
}


bool AudioAdapter::Init(){
	char buf[256];
	PaError err;

	f_init=true;
	inChan=CHANNELS;
	outChan=CHANNELS;
	framesPerBuffer=FRAMES_PER_BUFFER;

	Logger::Println("[AudioAdapter]	Initialize");

	err = Pa_Initialize();
	if( err != paNoError ) goto error;

	memset( &inputParameters , 0 , sizeof( inputParameters ) );
	inputParameters.channelCount = inChan;
	inputParameters.hostApiSpecificStreamInfo = NULL;
	inputParameters.sampleFormat = paFloat32;
	inputParameters.hostApiSpecificStreamInfo = NULL;

	memset( &outputParameters, 0 , sizeof( outputParameters ) );
	outputParameters.channelCount = outChan;
	outputParameters.hostApiSpecificStreamInfo = NULL;
	outputParameters.sampleFormat = paFloat32;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	hostApiCount = Pa_GetHostApiCount();
	devCount = Pa_GetDeviceCount();

	int usesetup=ApplicationProperty::ReadSetupInt("Audio","UseSetup",1);
	int useDefaultSetting=ApplicationProperty::ReadSetupInt("Audio","UseDefault",1);
	int settingInDev=ApplicationProperty::ReadSetupInt("Audio","InputDeviceNumber",Pa_GetDefaultInputDevice());
	int settingOutDev=ApplicationProperty::ReadSetupInt("Audio","OutputDeviceNumber",Pa_GetDefaultOutputDevice());

	if(usesetup==0){
		if(useDefaultSetting==1){
			inDevNum=Pa_GetDefaultInputDevice();
			outDevNum=Pa_GetDefaultOutputDevice();
		}else{
			inDevNum=settingInDev;
			outDevNum=settingOutDev;
		}

		inputParameters.device = inDevNum;
		outputParameters.device = outDevNum;
		if(inDevNum!=devCount){
			inputParameters.suggestedLatency = Pa_GetDeviceInfo(inDevNum)->defaultLowInputLatency ;
		}
		if(outDevNum!=devCount){
			outputParameters.suggestedLatency = Pa_GetDeviceInfo(outDevNum)->defaultLowOutputLatency ;
		}

		err = Pa_IsFormatSupported(
			inDevNum==devCount?NULL:&inputParameters
			,outDevNum==devCount?NULL:&outputParameters
			, SAMPLE_RATE );
		if( err != paNoError ){
			cout << "[AudioAdapter] Error: " << Pa_GetErrorText(err) << endl;
			usesetup=1;
		}
	}

	if(usesetup==1){
		do{

			hostApiNum=SelectApi();
			inDevNum=SelectInput();
			outDevNum=SelectOutput();

			inputParameters.device = inDevNum;
			outputParameters.device = outDevNum;
			if(inDevNum!=devCount){
				inputParameters.suggestedLatency = Pa_GetDeviceInfo(inDevNum)->defaultLowInputLatency ;
			}
			if(outDevNum!=devCount){
				outputParameters.suggestedLatency = Pa_GetDeviceInfo(outDevNum)->defaultLowOutputLatency ;
			}

			err = Pa_IsFormatSupported(
				inDevNum==devCount?NULL:&inputParameters
				,outDevNum==devCount?NULL:&outputParameters
				, SAMPLE_RATE );
			if( err != paNoError ){
				cout << "[AudioAdapter] Error: " << Pa_GetErrorText(err) << endl;
			}

		}while(err!=paNoError);
	}

	Logger::Println("[AudioAdapter]	Audio Info");
	if(0<=outDevNum && outDevNum<devCount){
		Logger::Println("	HostApi	: %s",Pa_GetHostApiInfo(Pa_GetDeviceInfo(outDevNum)->hostApi)->name);
	}
	if(devCount==inDevNum){
		Logger::Println("	No Input");
	}else{
		getDeviceType(Pa_GetDeviceInfo(inDevNum)->maxInputChannels,Pa_GetDeviceInfo(inDevNum)->maxOutputChannels,buf);
		Logger::Println("	Input Device	: %s",Pa_GetDeviceInfo(inDevNum)->name);
	}
	if(devCount==outDevNum){
		Logger::Println("	No Output");
	}else{
		getDeviceType(Pa_GetDeviceInfo(outDevNum)->maxInputChannels,Pa_GetDeviceInfo(outDevNum)->maxOutputChannels,buf);
		Logger::Println("	Output Device	: %s",Pa_GetDeviceInfo(outDevNum)->name);
	}
	ApplicationProperty::WriteSetupInt("Audio","InputDeviceNumber",inDevNum);
	ApplicationProperty::WriteSetupInt("Audio","OutputDeviceNumber",outDevNum);

	audioApi=new MyAudioApi;
	audioApi->Init();

	return true;

error:
	Logger::Println("[AudioAdapter]	Error : %s",Pa_GetErrorText(err));
	return false;
}

int AudioAdapter::SelectApi(){
	cout << "Query Supported Host Api" <<endl;
	int i,res=0;
	for(i=0;i<hostApiCount;i++){
			cout << "[" << i << "]	"<< Pa_GetHostApiInfo(i)->name << endl;
	}
	do{
		cout << "Choose Host Api : ";
		cin >> res;
	}while(! (0<=res && res<Pa_GetHostApiCount()) );

	return res;
}

int AudioAdapter::SelectInput(){
	int i,res,c;
	map<int,int> t;
	cout << "Query Supported Input Device" << endl;
	c=0;
	for(i=0;i<devCount;i++){
		devinfo=Pa_GetDeviceInfo(i);
		if(devinfo->hostApi==hostApiNum && devinfo->maxInputChannels >= 2){
			cout << "["<<c<<"] " << devinfo->name <<endl;
			t.insert(map<int,int>::value_type(c,i));
			c++;
		}
	}
	cout << "["<<c<<"] "<<"No Input"<<endl;
	t.insert(map<int,int>::value_type(c,devCount));
	c++;
	do{
		cout << "Choose Input Device : ";
		cin >> res;
	}while( ! ( 0<=res && res<c));
	return t[res];
}

int AudioAdapter::SelectOutput(){
	int i,res,c;
	map<int,int> t;
	cout << "Query Supported Output Device" << endl;
	c=0;
	for(i=0;i<devCount;i++){
		devinfo=Pa_GetDeviceInfo(i);
		if(devinfo->hostApi==hostApiNum && devinfo->maxOutputChannels >= 2){
			cout << "["<<c<<"] " << devinfo->name <<endl;
			t.insert(map<int,int>::value_type(c,i));
			c++;
		}
	}
//	cout << "["<<c<<"] "<<"No Output"<<endl;
//	t.insert(map<int,int>::value_type(c,devCount));
//	c++;
	do{
		cout << "Choose Output Device : ";
		cin >> res;
	}while( !(0<=res && res<c));
	return t[res];
}

void AudioAdapter::Run(){
	Logger::Println("[AudioAdapter]	Run");
	PaError err;
	if(!f_init)return;

	Logger::Debugln("[AudioAdapter]	Open Audio Stream");
	err = Pa_OpenStream(
		&stream,
		inDevNum==devCount?NULL:&inputParameters,
		outDevNum==devCount?NULL:&outputParameters,
		SAMPLE_RATE,
		framesPerBuffer,
		true,
		AudioAdapter::AudioCallback,
		this);
	if( err != paNoError ) goto error;

	f_open=true;

	Logger::Debugln("[AudioAdapter]	Start Audio Stream");
	err = Pa_StartStream(stream);
	if( err != paNoError ) goto error;

	f_start=true;

	return;

error:
	Logger::Println("[AudioAdapter]	AudioSetup error %s",Pa_GetErrorText(err));
	return;
}

void AudioAdapter::Close(){
	PaError err;
	Logger::Println("[AudioAdapter]	Close");
	if(f_start){
		Logger::Debugln("[AudioAdapter]	Stop Audio Stream");
		err = Pa_StopStream(stream);
		if( err != paNoError ) goto error;
	}

	if(f_start){
		Logger::Debugln("[AudioAdapter]	Close Audio Stream");
		err = Pa_CloseStream( stream );
		if( err != paNoError ) goto error;
	}

	Logger::Debugln("[AudioAdapter]	Terminate AudioStream");
	if(f_init){
		err = Pa_Terminate();
		if( err != paNoError ) goto error;
	}

	audioApi->Close();
	SAFE_DELETE(audioApi);


	return;
error:
	Logger::Println("[AudioAdapter]	Audio error %s",Pa_GetErrorText(err));
	return;
}


void AudioAdapter::getDeviceType(int in,int out,char* buf){
	if(in>=CHANNELS && out>=CHANNELS){
		sprintf_s(buf,256,"Input & Output");
	}else if(in>=CHANNELS){
		sprintf_s(buf,256,"Input");
	}else if(out>=CHANNELS){
		sprintf_s(buf,256,"Output");
	}else{
		sprintf_s(buf,256,"Can't Use");
	}
}

int AudioAdapter::AudioCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
	float* output = (float*) outputBuffer;
	float* input = (float*) inputBuffer;
	PackageManager::AudioCallbackProc(input,output,framesPerBuffer);
	return 0;
}

AudioApi* AudioAdapter::GetAPI(){
	return audioApi;
}