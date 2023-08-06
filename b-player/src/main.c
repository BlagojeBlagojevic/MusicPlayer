#include<assert.h>
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<raylib.h>
#include<string.h>
#include<complex.h>
#include<math.h>
#include<unistd.h>  // acces
#include<string.h>

#define ARRSIZE(ARR) sizeof(ARR)/sizeof(ARR[0])  //size of arrary 
float volume=0;
char text[65]="1.mp3";
char naslov[65]="1.mp3";
bool is_playing=false;
size_t global_text_counter=0;
float globalframes[48000];
size_t globalframesnum;  //Global frames callback is calld by sampling rate 48000 separate thread from main (Is not in begin end drawing)

void callback(void *buffer,uint16_t frames)
{
    if(frames>ARRSIZE(globalframes)) //limit for memcp
    {
        frames=ARRSIZE(globalframes);
    }
    memcpy(globalframes,buffer,frames*sizeof(float));//copy curent in global 
    globalframesnum=frames;
}



void Draw() //DRAW WAWE FORM
{
    float x=(float)ARRSIZE(globalframes)/GetScreenWidth(),y=(float)GetScreenHeight()/2;  // half y midle line cord mov 
    //DrawLine(670,840,y,y,GREEN);
    for (size_t i = 0; i < globalframesnum+100; i++)
    {
        /* code */
            float sample=globalframes[i];  //2 chanel same
            if(sample<0)   //if sample - just+ then print + down on x axis 
                DrawRectangle(i+650,y,1,(-1)*sample*(y),GREEN);

            else  //if difer
                DrawRectangle(i+650,y-sample*y,1,sample*(y),GREEN);  
        
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
    float x=0.1*(float)ARRSIZE(globalframes)/GetScreenWidth(),y=(float)GetScreenHeight()/2;  // half y midle line cord mov 
    float frequency;
    float complex ampritude[48000];
    float *sample;
    //memcpy(sample,globalframes,100);
    //printf("Nesto");
    //int samples=&globalframes;
    //DrawLine(0,y,400,y,GREEN);
    fft(globalframes,1,ampritude,400);
    for (size_t i = 0; i < 250; i++)
    {
        //ampritude[i]=log10f(ampritude[i]);
        //frequency=log10f((float)i);
        //frequency=i;
        
          float a=(1)*sqrtf(ampritude[i])*creal(ampritude[i])+cimag(ampritude[i])*cimag(ampritude[i]);
            DrawRectangle(i,y-a/2,4,a/2,GREEN);
            DrawRectangle(i,y,4,a/2,GREEN);  
  
    }
}


void Procces()
{
    int key = GetCharPressed();

            // Check if more characters have been pressed on the same frame
            while (key > 0)
            {
                // NOTE: Only allow keys in range [32..125]
                if ((key >= 32) && (key <= 125) && (global_text_counter < ARRSIZE(text)))
                {
                    text[global_text_counter] = (char)key;
                    text[global_text_counter+1] = '\0'; // Add null terminator at the end of the string.
                    global_text_counter++;
                }
                key = GetCharPressed();  // Check next character in the queue
            }

            if (IsKeyPressed(KEY_BACKSPACE)&&global_text_counter!=0) //delete
            {
                global_text_counter--;
                if (global_text_counter < 0) 
                    global_text_counter = 0;
                text[global_text_counter] = '\0';
            }
}
void Draw_input(Rectangle textBox)
{
    size_t i  =0 ;
    bool mouseOnText = false;
    if (CheckCollisionPointRec(GetMousePosition(), textBox)) 
        mouseOnText = true;
    else 
        mouseOnText = false;

   if(mouseOnText)
    {
       Procces();
    }
      if(text[0]=='\0' && !mouseOnText)
        {
            DrawText("Sta se slusa !!!",(int)textBox.x,(int)textBox.y,30,GREEN);
            DrawRectangleLines(textBox.x,textBox.y,textBox.width,textBox.height,GREEN);

        }
      else
        DrawText(text,(int)textBox.x,(int)textBox.y,30,GREEN);
}

Music Change_File(Music song)
{
    if(IsMusicStreamPlaying(song))
    {
        UnloadMusicStream(song);
    }
    if(IsAudioStreamProcessed(song.stream))
    {
        DetachAudioStreamProcessor(song.stream,callback);
    }
    song=LoadMusicStream(text);
    PlayMusicStream(song);
    AttachAudioStreamProcessor(song.stream,callback);
    ClearBackground(BLACK);
    SetMusicVolume(song,volume);

    for (size_t i = 0; i < ARRSIZE(naslov); i++)
    {
        /* code */
        naslov[i]=text[i];
    }
    text[0]='\0';
    global_text_counter=0;
    return song;   
}
void Draw_volume(Rectangle volumeBox,Music song)
{
    char pom[34];
                sprintf(pom,"volume:%d ",(int)(100*volume));
                DrawText(pom,volumeBox.x,volumeBox.y,30,GREEN);
                int key=GetKeyPressed();
                ///printf("%d",key);
                if(key==KEY_UP&&volume!=1.0f)
                {
                    volume+=0.025f;
                    SetMusicVolume(song,volume);

                }
                if(key==KEY_DOWN&&volume>=0.0f)
                {
                    SetMusicVolume(song,volume);
                    volume-=0.025;
                }
                if(volume<=0.0f)
                {
                    volume=0.0f;
                }
}
void Draw_time(Rectangle volumeBox,Music song)
{
    float duration=GetMusicTimeLength(song),passed=GetMusicTimePlayed(song);
    char pom[65];
    if(IsMusicStreamPlaying(song))
      {
          sprintf(pom,"%d : %d - %d : %d",((int)passed/60)%60,(int)passed%60,((int)duration/60)%60,(int)duration%60);
          DrawText(pom,600,volumeBox.y,30,GREEN);
      }   
}

void Draw_progres(Music song)
{
            //progres
                Vector2 A={220,545},B={220,565},C={230,555};
                Rectangle play={220,545,10,20};
                //CHECK COLISION
                if(CheckCollisionPointRec(GetMousePosition(),play))
                {
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        is_playing=!is_playing;
                    }

                }
                if(is_playing)
                {
                    //pause button
                    DrawLine(play.x,play.y,play.x,play.y+20,GREEN);
                    DrawLine(play.x+1,play.y,play.x+1,play.y+20,GREEN);
                    DrawLine(play.x+2,play.y,play.x+2,play.y+20,GREEN);
                    DrawLine(play.x+10,play.y,play.x+10,play.y+20,GREEN);
                    DrawLine(play.x+1+10,play.y,play.x+1+10,play.y+20,GREEN);
                    DrawLine(play.x+2+10,play.y,play.x+2+10,play.y+20,GREEN);
                    ResumeMusicStream(song);
                }
                else
                   {
                        PauseMusicStream(song);
                        DrawTriangle(A,B,C,GREEN);
                   }
                Rectangle progresbar={240,550,350,20};
                DrawRectangleLines(240,550,350,10,GREEN); 
                float percent=350*GetMusicTimePlayed(song)/GetMusicTimeLength(song);
                DrawRectangle(240,550,percent,10,GREEN);

                if(CheckCollisionPointRec(GetMousePosition(),progresbar)&&IsMusicStreamPlaying(song))
                {
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        PauseMusicStream(song);
                        float position=((GetMousePosition().x-240.0)/350)*GetMusicTimeLength(song);
                        SeekMusicStream(song, position);
                    }     
                }
}
int main(void)
{
    InitWindow(840,600,"NESTO");
    InitAudioDevice();
    SetTargetFPS(30);  //LOWER FPS BETHER VISual
    Music song;
    Rectangle volumeBox = { 100,500 ,300, 50, 50 };//same as DrawRectangle(y,x,with,height,color)
    Rectangle textBox = { 250,320-80,350, 50, 50 };//same as DrawRectangle(y,x,with,height,color)
    int x =GetScreenWidth(),y=GetScreenHeight();
    size_t i=0;
    song=LoadMusicStream(text);
    PlayMusicStream(song);
    AttachAudioStreamProcessor(song.stream,callback);
    SetMusicVolume(song,volume);
    while (!WindowShouldClose())
    {
            BeginDrawing();
                UpdateMusicStream(song);
                ClearBackground(BLACK);
                DrawText("B-PLAYER ! ! !",x/2-150,0,40,GREEN);
                DrawText(naslov,x-i,100,20,GREEN);
                i++;
                if(i==800)
                    i=0;
                Draw();
                Draw_fft();
                Draw_input(textBox);
                Draw_time(volumeBox,song);
                Draw_volume(volumeBox,song);
                Draw_progres(song);
                if(!access(text,F_OK))
                    song=Change_File(song);
            EndDrawing();
    }
    CloseWindow();   
    return 0;
}
