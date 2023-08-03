#include<assert.h>
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<raylib.h>
#include<string.h>
#include<complex.h>
#include<math.h>
#define ARRSIZE(ARR) sizeof(ARR)/sizeof(ARR[0])  //size of arrary 

float globalframes[20000]={0},globalampritudes[4800];
size_t globalframesnum;  //Global frames callback is calld by sampling rate 48000 separate thread from main (Is not in begin end drawing)

void callback(void *buffer,uint16_t frames)
{
    float complex out[4800];
    if(frames>ARRSIZE(globalframes)) //limit for memcp
    {
        frames=ARRSIZE(globalframes);
    }
    memcpy(globalframes,buffer,frames*sizeof(float));//copy curent in global 
}



void Draw() //DRAW WAWE FORM
{
    float x=(float)ARRSIZE(globalframes)/GetScreenWidth(),y=(float)GetScreenHeight()/2;  // half y midle line cord mov 
    //printf("%f ",x);
    DrawLine(0,y,x,y,GREEN);
    for (size_t i = 0; i < globalframesnum; i+=1)
    {
        /* code */
        if(i%2==0)
        {
            float sample=globalframes[i];  //2 chanel same
            if(sample<0)   //if sample - just+ then print + down on x axis 
                DrawRectangle(i*x,y,6,(-1)*sample*(y),GREEN);

            else  //if difer
                DrawRectangle(i*x,y-sample*y,6,sample*(y),GREEN);  
        }
    }
}
//n>0
//n==1 fourie of itself is it e^(-0I)=1
//DFT periodical for period of n/2 for cos even out[i+n/2]=out[i] for sin odd if out[i+n/2]=-out[i] 
//same marks ==
//# (*) -# (*) -# (*) # (*) | -# (*) # (*) # (*) -# (*) 
//calling fft for even geting nesesari numbers for futher stor in memori same for odd 
//store rest of the frequency in an obtained vars
//
void fft(float *in,size_t pivot,float complex *out,size_t n)  //coll
{
    float pi=3.141569;
    assert(n > 0);
    //printf("nesto");
    if(n==1)
    {
        out[0]=in[0];
        return; //braking assert error
    }
    fft(in,pivot*2,out,n/2);//even
    fft(in+pivot,pivot*2,n/2+out,n/2);//odd  
    for (size_t k = 0; k < n/2; k++)
    {
        float t=(float)k/n;
        float complex e = out[k];
        float complex v = cexp(-2*pi*I*t)*out[k+n/2];
        out[k]=e+v;
        out[k+n/2]=e-v; 
    }
    //for (size_t i = 0; i < n; i++)
    //{
    //    /* code */
    //    printf("%f %f\n",creal(out[i]),cimag(out[i]));
    //}
    
    
}

void Draw_fft() //DRAW WAWE FORM
{
    float x=(float)ARRSIZE(globalframes)/GetScreenWidth(),y=(float)GetScreenHeight()/2;  // half y midle line cord mov 
    float frequency;
    float complex ampritude[20000];
    float *sample;
    //memcpy(sample,globalframes,100);
    //printf("Nesto");
    //int samples=&globalframes;
    DrawLine(0,y,840,y,GREEN);
    fft(globalframes,1,ampritude,840);
    for (size_t i = 0; i < 840; i++)
    {
        frequency=log10f((float)i);
        //frequency=i;
        
          float a=(1)*sqrtf(ampritude[i])*creal(ampritude[i])+cimag(ampritude[i])*cimag(ampritude[i]);
            DrawRectangle(i,320-a,frequency,a,GREEN);  
    }
}


int main(void)
{
    InitWindow(840,620,"NESTO");
    InitAudioDevice();
    SetTargetFPS(30);  //LOWER FPS BETHER VISual
    Music song=LoadMusicStream("MIX NARODNE MUZIKE ZA DINAMICNU VOZNJU (2022).mp3");
    PlayMusicStream(song);  //2 chanel 48000 sampling rate 32 bits
    SetMusicVolume(song,0.0f);
    AttachAudioStreamProcessor(song.stream,callback);

    //printf("%u ",song.stream.channels);

    while (!WindowShouldClose())
    {
            BeginDrawing();
                UpdateMusicStream(song);
                ClearBackground(RAYWHITE);
                //Draw();
                Draw_fft();
            EndDrawing();
    }
    CloseWindow();   
    return 0;
}
