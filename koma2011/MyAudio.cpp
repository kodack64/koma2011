
#include "MyAudio.h"
#include "Logger.h"
void MyAudio::Init(AudioId _id){
	stereo=false;
	wsr=new WaveShell;
	wsl=new WaveShell;
	id=_id;
	loadState=Audio_None;
	state=Audio_Stop;
	
	fade=1.0f;
	speed=0.00001f;
	vol=1.0f;
}
void MyAudio::Close(){
	SAFE_DELETE(wsr);
	SAFE_DELETE(wsl);
}
void MyAudio::Clear(){
	wsr->Clear();
	wsl->Clear();
	loadState=Audio_None;
}
AudioState MyAudio::GetState(){
	return state;
}
AudioLoadState MyAudio::GetLoadState(){
	return loadState;
}
void MyAudio::Rewind(){
	if(wsr!=NULL){
		wsr->SetCursor(0);
	}
	if(wsl!=NULL){
		wsl->SetCursor(0);
	}
}

void MyAudio::SetState(AudioState _state){
	state=_state;
	switch(_state){
	case Audio_Play:
	case Audio_Stop:
	case Audio_LoopPlay:
		break;
	case Audio_FadeOut:
	case Audio_FadeOutAndLoop:
		fade=1.0f;
		break;
	case Audio_FadeIn:
	case Audio_FadeInAndLoop:
		fade=0.0f;
		break;
	default:
		break;
	}
}

AudioId MyAudio::LoadFile(string filepath){
	loadState=Audio_None;
	state=Audio_Stop;

	mypath=filepath;
	FILE* wlfp;
	BYTE id[4];
	unsigned long file_size,fmt_size,sample_rate,avg_bytes_sec,data_size;
	unsigned short tag,channels,block_align,bits_per_sample;
	fopen_s(&wlfp,filepath.c_str(),"rb");
	if(wlfp==NULL){
		Logger::Println("[AudioApi]	Load Fail : File not Found");
		return -1;
	}
	fread(id,sizeof(BYTE),4,wlfp);
	if(memcmp(id,"RIFF",4)!=0){
		Logger::Println("[AudioApi]	Load Fail : No RIFF ID");
		fclose(wlfp);
		return -1;
	}
	fread(&file_size,sizeof(DWORD),1,wlfp);
	fread(id,sizeof(BYTE),4,wlfp);
	if(memcmp(id,"WAVE",4)!=0){
		Logger::Println("[AudioApi] Load Fail : No WAVE Id");
		fclose(wlfp);
		return -1;
	}
	fread(id,sizeof(BYTE),4,wlfp);
	if(memcmp(id,"fmt ",4)!=0){
		Logger::Println("[AudioApi] Load Fail : No fmt ID");
		fclose(wlfp);
		return -1;
	}

    fread(&fmt_size, sizeof(DWORD),1,wlfp);
    fread(&tag, sizeof(short), 1, wlfp);
	if(tag!=0x0001){
		Logger::Println("[AudioApi] Load Fail : Not PCM WAVE");
		fclose(wlfp);
		return -1;
	}
    fread(&channels, sizeof(short),1,wlfp);
    fread(&sample_rate, sizeof(DWORD), 1, wlfp);
    fread(&avg_bytes_sec, sizeof(DWORD), 1, wlfp);
    fread(&block_align, sizeof(short), 1, wlfp);
    fread(&bits_per_sample, sizeof(short), 1, wlfp); 

	fread(id,sizeof(BYTE),4,wlfp);
	if(memcmp(id,"data",4)!=0){
		Logger::Println("[AudioApi] Load Fail : No data ID");
		fclose(wlfp);
		return -1;
	}
	fread(&data_size, sizeof(DWORD), 1, wlfp); 

	Logger::Debugln("[AudioApi] File Info :%s",filepath.c_str());
	Logger::Debugln("	Channel	:%d",channels);
	Logger::Debugln("	SampleRate	:%d",sample_rate);
	Logger::Debugln("	BitPerSample	:%d",bits_per_sample);
	Logger::Debugln("	DataSize	:%d",data_size);
	if(sample_rate!=44100){
		Logger::Println("[AudioApi] Load Fail : Not Supported Sample Rate (supported only 44100)");
		fclose(wlfp);
		return -1;
	}
	if(bits_per_sample!=8 && bits_per_sample!=16){
		Logger::Println("[AudioApi] Load Fail : Not Supported Bits_Per_Sample (supported only 8 or 16)");
		fclose(wlfp);
		return -1;
	}

	if(channels==1){
		stereo=false;
	}else{
		stereo=true;
	}

	wsl->Init();
	wsr->Init();
	wsl->Resize(data_size/channels/(bits_per_sample/8));
	wsr->Resize(data_size/channels/(bits_per_sample/8));

	unsigned long i,j;
	short spos;
	float pos;
	unsigned char a,b;
	for(i=0;i<data_size;){
		for(j=0;j<(unsigned)channels;j++){
			if(bits_per_sample==8){
				spos=GetNextData(wlfp)-128;
				pos=spos/128.0f;
				i++;
			}else if(bits_per_sample==16){
				a=GetNextData(wlfp);
				b=GetNextData(wlfp);
				spos=(short)(a+b*256);
				pos=spos/32768.0f;
				i+=2;
			}
			if(channels==1){
				wsl->SetCursorPos(pos);
				wsr->SetCursorPos(pos);
				wsl->Forward();
				wsr->Forward();
			}else{
				if(j==0){
					wsl->SetCursorPos(pos);
					wsl->Forward();
				}else{
					wsr->SetCursorPos(pos);
					wsr->Forward();
				}
			}
		}
	}
	Logger::Println("[AudioApi] Load Success");
	loadState=Audio_Loaded;
	fclose(wlfp);
	return 0;
}

bool MyAudio::WriteFile(string filepath){
	FILE* wwfp;
	BYTE id[4];
	unsigned long file_size,fmt_size,sample_rate,avg_bytes_sec,data_size,header_size;
	unsigned short tag,channels,block_align,bits_per_sample;

	sample_rate=SAMPLE_RATE;
	channels=2;
	bits_per_sample=16;
	block_align=channels*bits_per_sample/8;
	avg_bytes_sec=block_align*sample_rate;
	data_size=block_align*min(wsl->GetLength(),wsr->GetLength());
	header_size=44;
	file_size=data_size+header_size-8;
	fmt_size=16;
	tag=1;

	fopen_s(&wwfp,filepath.c_str(),"wb");
	if(wwfp==NULL)return false;

	/* riff */
	memcpy(id,"RIFF",4);
	BYTE id2[4];
	id2[0]='R';
	id2[1]='I';
	id2[2]='F';
	id2[3]='F';
	fwrite(id,1,4,wwfp);
	fwrite(&file_size, sizeof(file_size), 1, wwfp);
	memcpy(id,"WAVE",4);
	fwrite(id,1,4,wwfp);

	/* fmt */
	memcpy(id,"fmt ",4);
	fwrite(id,1,4,wwfp);
	fwrite(&fmt_size, sizeof(fmt_size), 1, wwfp);
	fwrite(&tag, sizeof(tag), 1, wwfp);
	fwrite(&channels, sizeof(channels), 1, wwfp);
	fwrite(&sample_rate, sizeof(sample_rate), 1, wwfp);
	fwrite(&avg_bytes_sec, sizeof(avg_bytes_sec), 1, wwfp);
	fwrite(&block_align, sizeof(block_align), 1, wwfp);
	fwrite(&bits_per_sample, sizeof(bits_per_sample), 1, wwfp);

	/* data */
	memcpy(id,"data",4);
	fwrite(id,1,4,wwfp);
	fwrite(&data_size, sizeof(data_size), 1, wwfp);

	WAVEPOS n,sn;
	float fs;
	short ss;
	for (n=sn=0;n<(signed)data_size;sn++)
	{
		fs=wsl->GetPos(sn)*32768.0f;
		if (fs<-32768.0f)	fs=-32768.0f;
		if (fs>32767.0f)	fs=32767.0f;
		ss = (short)(fs+0.5);
		fwrite(&ss,sizeof(ss),1,wwfp);
		n+=2;

		fs=wsr->GetPos(sn)*32768.0f;
		if (fs<-32768.0f)	fs=-32768.0f;
		if (fs>32767.0f)	fs=32767.0f;
		ss = (short)(fs+0.5);
		fwrite(&ss,sizeof(ss),1,wwfp);
		n+=2;
	}
  	fclose(wwfp);
	Logger::Println("[AudioApi] Write Success");
	return 0;
}

unsigned char MyAudio::GetNextData(FILE* myfp){
	unsigned char buf;
	fread(&buf,sizeof(unsigned char),1,myfp);
	return buf;
}

bool MyAudio::IsPlay(){
	if(state!=Audio_Stop && loadState==Audio_Loaded){
		return true;
	}else{
		return false;
	}
}

void MyAudio::GetWaveData(float* left,float* right){
	float rf;
	if(state==Audio_FadeIn || state==Audio_FadeInAndLoop){
		rf=fade;
		fade+=speed;
		if(fade>=1.0f){
			fade=1.0f;
			if(state==Audio_FadeIn){
				state=Audio_Play;
			}else{
				state=Audio_LoopPlay;
			}
		}
	}else if(state==Audio_FadeOut || state==Audio_FadeOutAndLoop){
		rf=fade;
		fade-=speed;
		if(fade<=0.0f){
			fade=0.0f;
			state=Audio_Stop;
		}
	}else{
		rf=1.0f;
	}
	(*left)=wsl->GetCursorPos()*rf*vol;
	(*right)=wsr->GetCursorPos()*rf*vol;
	wsl->Forward();
	wsr->Forward();
	if(wsl->GetCursor()==0){
		if(state==Audio_Play || state==Audio_FadeIn || state==Audio_FadeOut){
			state=Audio_Stop;
		}
	}
}
WaveShell* MyAudio::GetWaveShellLeft(){
	return wsl;
}
WaveShell* MyAudio::GetWaveShellRight(){
	return wsr;
}

void MyAudio::SetFadeSpeed(float sp){
	speed=sp;
}
