#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<raylib.h>
#include<string.h>
#define ARRSIZE(ARR) sizeof(ARR)/sizeof(ARR[0])  //size of arrary 

float globalframes[4800]={0};
size_t globalframesnum;  //Global frames callback is calld by sampling rate 48000 separate thread from main (Is not in begin end drawing)

void callback(void *buffer,uint16_t frames)
{
    if(frames>ARRSIZE(globalframes)) //limit for memcp
    {
        frames=ARRSIZE(globalframes);
    }
    memcpy(globalframes,buffer,frames*sizeof(float));//copy curent in global 
    globalframesnum=frames;  //how many for display
    //for (size_t i = 0; i < ARRSIZE(globalframes); i++)
    //{
    //    int16_t sample=*(int16_t*)&globalframes[i];
    //    printf("%u ",sample);
    //}
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

int main(void)
{
    InitWindow(840,620,"NESTO");
    InitAudioDevice();
    SetTargetFPS(60);  //LOWER FPS BETHER VISual
    Music song=LoadMusicStream("location");
    PlayMusicStream(song);  //2 chanel 48000 sampling rate 32 bits
    SetMusicVolume(song,0.0f);
    AttachAudioStreamProcessor(song.stream,callback);

    //printf("%u ",song.stream.channels);

    while (!WindowShouldClose())
    {
            BeginDrawing();
                UpdateMusicStream(song);
                ClearBackground(RAYWHITE);
                Draw();
            EndDrawing();
    }
    CloseWindow();   
    return 0;
}