#include "stdlib.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"

#define FPS 20
#define VELO 10
#define GRAVIDADE 10

TTF_Font* my_font;
#define FONTSIZE 18


int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    if(TTF_Init() == -1) return -1;
    my_font = TTF_OpenFont("myfont.ttf", FONTSIZE);
    SDL_Color cor = {255, 255, 255}; //vermelho
    SDL_Surface* screen;
    SDL_Surface* imagem;
    SDL_Surface* fundo;
    SDL_Surface* bolafogo;
    SDL_Surface* bolafogo2;
    SDL_Surface* bolafogo3;
    SDL_Surface* bolafogo4;
    SDL_Surface* BOSS;
    SDL_Surface* princesa;
    SDL_Surface* monstro;
    SDL_Surface* monstro2;
    SDL_Surface* monstro3;
    SDL_Surface* monstro4;
    SDL_Surface* barreira;
    SDL_Surface* risada = NULL;
    SDL_Surface* src2 = NULL;
    SDL_Surface* hsrc1 = NULL;
    SDL_Surface* hsrc2 = NULL;
    SDL_Surface* tela = NULL;

    SDL_Rect dest,dest1,p1,bola,bola2,bola3,bola4,fogo[4],b1,b2,b3,prince,mov1,mov2,mov3,mov4,mov5,prot;
    SDL_Surface* lava;
    int velX=0, velY=0, pX=0, pY=0, fase=0, auY=0,plataforma = false,escada=false,desce=0,contador=0, start,tempo=0,gameover=false,concluido=false,bar=false,ponto = 0;
    char score[6],hscore[6];

    FILE *recorde = fopen("recorde.txt","r");
    int r1,ponto1=true,ponto2=true,ponto3=true,ponto4=true;

    fscanf(recorde,"%d",&r1);

    fclose(recorde);

     Mix_CloseAudio();

    //Musica

     if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
      fprintf(stderr, "Nao foi possivel inicializar o SDL: %s", SDL_GetError());
      return 1;
    }

   int audio_rate = 22050;
   /* 22050 é o ideal para a frequência na maioria dos jogos. Se o seu objetivo for criar algum GuitarHero, talvez seja melhor aumentar esse valor. A qualidade de CD é 44100. */

   Uint16 audio_format = AUDIO_S16SYS; /* Ajuste de amostras com 16 bits. Pode ser necessário alterar para 8 bits: AUDIO_S8 */
   int audio_channels = 2; /* 1-Mono; 2-Stereo */
   int audio_buffers = 4096; /* Buffer para armazenamento de trechos do audio*/

   /* Mix_OpenAudio inicializa o sistema de audio de acordo com as configurações estabelecidas acima */
   if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
      fprintf(stderr, "Nao foi possivel inicializar o audio: %s\n", Mix_GetError());
      exit(1);
    }

   /* Ponteiro que receberá a amostra de audio do arquivo WAV e armazenará essa amostra na memória */
   Mix_Chunk *sound = NULL;

   sound = Mix_LoadWAV("menu.wav"); /* Carrega a música a partir do arquivo WAV) */
   if (sound == NULL) {
      fprintf(stderr, "Impossível carregar arquivo WAV: %s\n", Mix_GetError());
   }

     int channel;
   channel = Mix_PlayChannel(-1, sound, -1);
   if (channel == -1) {
      fprintf(stderr, "Impossível reproduzir arquivo WAV: %s\n", Mix_GetError());
   }







    screen = SDL_SetVideoMode(1024,768,32,SDL_SWSURFACE);
    imagem = IMG_Load("semibotao.png");
    SDL_SetColorKey(imagem, SDL_SRCCOLORKEY, SDL_MapRGB(imagem->format, 255, 255, 255));
    fundo = IMG_Load("menu.png");
    BOSS = NULL;
    barreira = NULL;
    princesa = NULL;
    monstro = NULL;
    monstro2 = NULL;
    monstro3 = NULL;
    monstro4 = NULL;

    bolafogo = NULL;
    bolafogo2 = NULL;
    bolafogo3 = NULL;
    bolafogo4 = NULL;


    dest.x=350;
    dest.y=350;

    dest1.x=0;
    dest1.y=0;
    dest1.w=382;
    dest1.h=75;

    p1.x=0;
    p1.y=800;

    mov1.x = 900;
    mov1.y = 0;

    mov2.x = 200;
    mov2.y = 100;

    mov3.x = 513;
    mov3.y = 300;

    mov4.x = 426;
    mov4.y = 450;

    mov5.x = 0;
    mov5.y = 0;
    mov5.w = 32;
    mov5.h = 60;

    prot.x = 500;
    prot.y = 500;

    score[0] = '0' + ponto/10000;
    score[1] = '0' + (ponto%10000)/1000;
    score[2] = '0' + ((ponto%10000)%1000)/100;
    score[3] = '0' + (((ponto%10000)%1000)%100)/10;
    score[4] = '0' + (((ponto%10000)%1000)%100)%10;
    score[5] = '\0';

    hscore[0] = '0' + r1/10000;
    hscore[1] = '0' + (r1%10000)/1000;
    hscore[2] = '0' + ((r1%10000)%1000)/100;
    hscore[3] = '0' + (((r1%10000)%1000)%100)/10;
    hscore[4] = '0' + (((r1%10000)%1000)%100)%10;
    hscore[5] = '\0';

    ponto1 = true;
    ponto2 = true;
    ponto3 = true;
    ponto4 = true;

   SDL_Event event;
   int done = 0;
   while(!done){

    /**********************************************
    ***********************************************
    ***                                         ***
    ***           COMANDOS DO TECLADO           ***
    ***                                         ***
    ***********************************************
    ***********************************************/

        start = SDL_GetTicks();
       while(SDL_PollEvent(&event))
       {
            if(event.type==SDL_QUIT)
            done=1;

            if (event.type == SDL_KEYDOWN)
            {

                switch (event.key.keysym.sym)
                {
                case SDLK_SPACE:
                    if(fase == 0)
                    {
                        if(dest.y == 350)
                            {

                                fase = 1;
                                imagem = IMG_Load("personagem2.bmp");
                                SDL_SetColorKey(imagem, SDL_SRCCOLORKEY, SDL_MapRGB(imagem->format, 255, 255, 255));
                                BOSS = IMG_Load("BOSS.png");
                                fundo = IMG_Load("fundo.png");
                                lava = IMG_Load("lava.png");
                                monstro = IMG_Load("monstro1.png");
                                monstro2 = IMG_Load("monstro2.png");
                                monstro3 = IMG_Load("monstro3.png");
                                monstro4 = IMG_Load("monstro4.png");
                                princesa = IMG_Load("princesa.png");
                                barreira = IMG_Load("barreira.png");
                                SDL_SetAlpha( barreira , SDL_RLEACCEL | SDL_SRCALPHA , 100);
                                dest.x = 0;
                                dest.y = 700;
                                dest1.w = 50;
                                ponto = 1000;
                            }
                        if(dest.y == 450)
                            {
                                fundo = IMG_Load("credito.png");
                                dest.x = 608;
                                dest.y = 677;
                                fase = 2;
                            }
                        if(dest.y == 540)
                            {
                                fundo = IMG_Load("comandos.png");
                                dest.x = 608;
                                dest.y = 677;
                                fase = 2;
                            }

                        if(dest.y == 625)
                            done = 1;
                    }
                    else if(fase == 2)
                    {
                        fundo = IMG_Load("menu.png");
                        dest.x=350;
                        dest.y=350;
                        fase = 0;
                    }

                    else if(fase == 1)
                            if(plataforma)
                            {
                                dest1.x = 0;
                                auY = 10;
                            }
                    break;

                case SDLK_UP:
                    if(fase==0)
                    {
                        if(dest.y==350)
                            dest.y = 625;
                        else if(dest.y==450)
                            dest.y = 350;
                        else if(dest.y==540)
                            dest.y = 450;
                        else if(dest.y==625)
                            dest.y = 540;
                    }
                    else if(fase==1)
                    {
                         if(escada){
                            velY = -VELO;
                            pX = 66;
                            }
                    }
                    break;

                case SDLK_DOWN:
                    if(fase==0)
                    {
                        if(dest.y==350)
                            dest.y = 450;
                        else if(dest.y==450)
                            dest.y = 540;
                        else if(dest.y==540)
                            dest.y = 625;
                        else if(dest.y==625)
                            dest.y = 350;
                    }
                    else if(fase==1)
                    {
                        if(escada||desce!=0){
                            velY = VELO;
                            pX = 66;

                            if(dest.y < desce)
                                dest.y = desce;
                        }
                    }
                    break;

                case SDLK_RIGHT:
                        velX = VELO;
                        pX= 66;
                        pY= 0;

                    break;

                case SDLK_LEFT:
                    velX = -VELO;
                    pY= 150;
                    pX= 66;
                    break;

                default:
                    break;
          }
        }


        if (event.type == SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_UP:
                    velY = 0;
                    if(velX==0)
                        pX = 0;
                    break;

                case SDLK_DOWN:
                    velY = 0;
                    if(velX==0)
                        pX = 0;
                    break;

                case SDLK_RIGHT:
                    if(velX>0){
                        velX = 0;

                        if(!escada)
                            pX = 0;

                        if(auY==0)
                            dest1.x = 0;
                    }
                    break;

                case SDLK_LEFT:
                     if(velX<0){
                        velX = 0;

                        if(!escada)
                            pX = 0;

                        if(auY==0)
                            dest1.x = 0;
                    }
                    break;

                default:
                    break;
          }
        }
     }
     if(fase==0)
     {
        if(contador > 2)
        {
            dest1.x += 382;
            contador = 0;

            if(dest1.x>1528)
                dest1.x = 0;
        }

        contador++;

        imagem = SDL_LoadBMP("semibotao.bmp");
        SDL_SetColorKey(imagem, SDL_SRCCOLORKEY, SDL_MapRGB(imagem->format, 255, 255, 255));

        princesa = NULL;
        monstro = NULL;
        monstro2 = NULL;
        monstro3 = NULL;
        monstro4 = NULL;

        fogo[0].x = 0;
        fogo[0].y = 130;
        fogo[0].w = 99;
        fogo[0].h = 15;

        fogo[1].x = 0;
        fogo[1].y = 130;
        fogo[1].w = 99;
        fogo[1].h = 15;

        fogo[2].x = 0;
        fogo[2].y = 130;
        fogo[2].w = 99;
        fogo[2].h = 15;

        fogo[3].x = 0;
        fogo[3].y = 130;
        fogo[3].w = 99;
        fogo[3].h = 15;

        mov1.x = 900;
        mov1.y = 0;

        mov2.x = 200;
        mov2.y = 100;

        mov3.x = 513;
        mov3.y = 300;

        mov4.x = 426;
        mov4.y = 450;

        mov5.x = 0;
        mov5.y = 0;
        mov5.w = 32;
        mov5.h = 60;

        b2.x = 874;
        b2.y = 0;

        b3.x = 602;
        b3.y = 0;

        b1.x = 0;
        b1.y = 0;
        b1.w = 150;
        b1.h = 115;

        prince.x = 975;
        prince.y = 120;

        bola.x = 20;
        bola.y = 0;

        bola2.x = 400;
        bola2.y = 0;

        bola3.x = 700;
        bola3.y = 0;

        bola4.x = 200;
        bola4.y = 0;

        p1.x=0;
        p1.y=1000;

        prot.x = 500;
        prot.y = 500;

        hscore[0] = '0' + r1/10000;
        hscore[1] = '0' + (r1%10000)/1000;
        hscore[2] = '0' + ((r1%10000)%1000)/100;
        hscore[3] = '0' + (((r1%10000)%1000)%100)/10;
        hscore[4] = '0' + (((r1%10000)%1000)%100)%10;
        hscore[5] = '\0';

        ponto1 = true;
        ponto2 = true;
        ponto3 = true;
        ponto4 = true;


     }
     else if(fase==2)
     {
            if(contador == 3)
        {
            dest1.x += 382;
            contador = 0;

            if(dest1.x>1528)
                dest1.x = 0;
        }

        contador++;
     }
     else if(fase==1)
     {

    /**********************************************
    ***********************************************
    ***                                         ***
    ***         COMANDOS DO PERSONAGEM          ***
    ***                                         ***
    ***********************************************
    ***********************************************/
    if(!gameover&&!concluido){
        if(auY==0)
        {
            if(velX==0&&velY==0)
            {
                dest1.x = 0;
                pX = 0;
            }

            if(plataforma)
            {
                dest1.y = pY;

                if(dest1.x>985)
                    dest1.x=66;
            }
            else if(escada)
            {
                dest1.y = 300;

                if(dest1.x>396)
                    dest1.x = 0;

                if(dest1.x<0)
                    dest1.x = 462;
            }
            else {
                dest1.y = pY +75;
                dest1.x = 528;
                }

            dest.x = dest.x + velX; // Soma a velocidade X
            dest1.x = dest1.x + pX;

            if(!escada)
                dest.y = dest.y + GRAVIDADE; // Soma a velocidade Y
            else
                dest.y = dest.y + velY;
        }
        else if(auY>0)
        {
                if(dest1.x>528)
                dest1.x=528;

            dest.x = dest.x + velX;
            dest.y = dest.y - 8;
            dest1.x= dest1.x + 66;
            dest1.y = pY +75;
            auY--;
        }

    /**********************************************
    ***********************************************
    ***                                         ***
    ***           COMANDOS DE COLISÃO           ***
    ***                                         ***
    ***********************************************
    ***********************************************/

        if(auY==0){
            if(dest.y+35<755&&dest.y+dest1.h+1>755&&dest.x+dest1.w>=0&&dest.x<1024){
                dest.y= 755-dest1.h;
                plataforma = true;
            }
            else if(dest.y+35<635&&dest.y+dest1.h+1>635&&dest.x+dest1.w>=0&&dest.x<928){
                dest.y= 635-dest1.h;
                plataforma = true;

            }
            else if(dest.y+35<505&&dest.y+dest1.h+1>505&&dest.x+dest1.w>=120&&dest.x<1024){
                dest.y= 505-dest1.h;
                plataforma = true;

            }
            else if(dest.y+35<365&&dest.y+dest1.h+1>365&&dest.x+dest1.w>=0&&dest.x<890){
                dest.y= 365-dest1.h;
                plataforma = true;

            }
            else if(dest.y+35<215&&dest.y+dest1.h+1>215&&dest.x+dest1.w>=51&&dest.x<1024){
                dest.y= 215-dest1.h;
                plataforma = true;

            }
            else if(dest.y+35<0&&dest.y+dest1.h+1>0&&dest.x+dest1.w>=0&&dest.x<0){
                    dest.y=0-dest1.h;
                    plataforma = true;

            }
            else
                plataforma = false;
        }
        else
            plataforma = false;

        if(dest.x+dest1.w>1024)
        {
            dest.x = 1024-dest1.w;
        }

    /**********************************************
    ***********************************************
    ***                                         ***
    ***            COLISÃO COM ESCADA           ***
    ***                                         ***
    ***********************************************
    ***********************************************/

        if(dest.y>580&&dest.y+dest1.h<765&&dest.x>860&&dest.x+30<930){
            escada = true;

            if(plataforma&&velX==0){
                pX = 0;
                dest1.x = 0;
            }
        }
        else if(dest.y>460&&dest.y+dest1.h<640&&dest.x>115&&dest.x+30<195){
            escada = true;

            if(plataforma&&velX==0){
                pX = 0;
                dest1.x = 0;
            }
        }
        else if(dest.y>310&&dest.y+dest1.h<520&&dest.x>806&&dest.x+30<875){
            escada = true;
            if(plataforma&&velX==0){
                pX = 0;
                dest1.x = 0;
            }
        }
        else if(dest.y>160&&dest.y+dest1.h<370&&dest.x>45&&dest.x+30<115){
            escada = true;

            if(plataforma&&velX==0){
                pX = 0;
                dest1.x = 0;
            }
        }
        else{
                escada = false;
                velY = 0;
            }

        desce = 0;
        if(dest.y>0&&dest.x>55&&dest.x<115){
            if(plataforma&&velX==0)
                desce = 190;
        }
        if(dest.y>230&&dest.x>810&&dest.x<870){
            if(plataforma&&velX==0)
                desce = 335;
        }
        if(dest.y>375&&dest.x>125&&dest.x<185){
            if(plataforma&&velX==0)
                desce = 485;
        }
        if(dest.y>540&&dest.x>865&&dest.x<925){
            if(plataforma&&velX==0)
                desce = 610;
        }

    /**********************************************
    ***********************************************
    ***                                         ***
    ***               BOLAS DE FOGO             ***
    ***                                         ***
    ***********************************************
    ***********************************************/

    if(fogo[0].y>0){
        fogo[0].y -=15;
        fogo[0].h +=15;
        if(fogo[0].y<0)
            fogo[0].y = 0;
        if(fogo[0].h>145)
            fogo[0].h = 145;
   }
   else
        bola.y += 15;

    if(fogo[1].y>0){
        fogo[1].y -=15;
        fogo[1].h +=15;
        if(fogo[1].y<0)
            fogo[1].y = 0;
        if(fogo[1].h>145)
            fogo[1].h = 145;
   }
   else
        bola2.y += 15;

    if(fogo[2].y>0){
        fogo[2].y -=15;
        fogo[2].h +=15;
        if(fogo[2].y<0)
            fogo[2].y = 0;
        if(fogo[2].h>145)
            fogo[2].h = 145;
   }
   else
        bola3.y += 15;

    if(fogo[3].y>0){
        fogo[3].y -=15;
        fogo[3].h +=15;
        if(fogo[3].y<0)
            fogo[3].y = 0;
        if(fogo[3].h>145)
            fogo[3].h = 145;
   }
   else
        bola4.y += 15;

    if(bola.y > 900){
        bola.y = 0;
        bolafogo = IMG_Load("boladefogo.png");
        fogo[0].y = 130;
        fogo[0].h = 15;
        if(bola.x == 20)
            bola.x = 300;
        else
            bola.x = 20;
    }

    if(bola2.y > 700){
        bola2.y = 0;
        bolafogo2 = IMG_Load("boladefogo.png");
        fogo[1].y = 130;
        fogo[1].h = 15;
        if(bola2.x == 400)
            bola2.x = 800;
        else
            bola2.x = 400;
    }

     if(bola3.y > 800){
        bola3.y = 0;
        bolafogo3 = IMG_Load("boladefogo.png");
        fogo[2].y = 130;
        fogo[2].h = 15;
         if(bola3.x == 700)
            bola3.x = 900;
        else
            bola3.x = 700;
    }

     if(bola4.y > 1000){
        bola4.y = 0;
        bolafogo4 = IMG_Load("boladefogo.png");
        fogo[3].y = 130;
        fogo[3].h = 15;
         if(bola4.x == 200)
            bola4.x = 500;
        else
            bola4.x = 200;
    }

    /**********************************************
    ***********************************************
    ***                                         ***
    ***        COLISÃO COM A BOLA DE FOGO       ***
    ***                                         ***
    ***********************************************
    ***********************************************/

     if(dest.x+dest1.w>bola.x+24&&dest.x<bola.x+fogo[0].w-28&&dest.y+33<bola.y+fogo[0].h-23&&dest.y+dest.h>bola.y+14&&bolafogo!=NULL){
        if(bar){
            bar = false;
            prot.x = 500;
            prot.y = 550;
            bolafogo = NULL;
            ponto+=25;


                            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
                            fprintf(stderr, "Nao foi possivel inicializar o SDL: %s", SDL_GetError());
                            return 1;
                            }

                        int audio_rate = 22050;
                        /* 22050 é o ideal para a frequência na maioria dos jogos. Se o seu objetivo for criar algum GuitarHero, talvez seja melhor aumentar esse valor. A qualidade de CD é 44100. */

                        Uint16 audio_format = AUDIO_S16SYS; /* Ajuste de amostras com 16 bits. Pode ser necessário alterar para 8 bits: AUDIO_S8 */
                        int audio_channels = 2; /* 1-Mono; 2-Stereo */
                        int audio_buffers = 4096; /* Buffer para armazenamento de trechos do audio*/

                        /* Mix_OpenAudio inicializa o sistema de audio de acordo com as configurações estabelecidas acima */
                        if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
                            fprintf(stderr, "Nao foi possivel inicializar o audio: %s\n", Mix_GetError());
                            exit(1);
                        }

                        /* Ponteiro que receberá a amostra de audio do arquivo WAV e armazenará essa amostra na memória */
                        Mix_Chunk *sound = NULL;

                        sound = Mix_LoadWAV("collect-coin.wav"); /* Carrega a música a partir do arquivo WAV) */
                        if (sound == NULL) {
                            fprintf(stderr, "Impossível carregar arquivo WAV: %s\n", Mix_GetError());
                        }

                        int channel;
                        channel = Mix_PlayChannel(-1, sound, 0);
                        if (channel == -1) {
                            fprintf(stderr, "Impossível reproduzir arquivo WAV: %s\n", Mix_GetError());
                        }
        }
        else
            gameover = true;
    }

    if(dest.x+dest1.w>bola2.x+24&&dest.x<bola2.x+fogo[1].w-28&&dest.y+33<bola2.y+fogo[1].h-23&&dest.y+dest.h>bola2.y+14&&bolafogo2!=NULL){
        if(bar){
            bar = false;
            prot.x = 500;
            prot.y = 550;
            bolafogo2 = NULL;
            ponto+=25;


                            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
                            fprintf(stderr, "Nao foi possivel inicializar o SDL: %s", SDL_GetError());
                            return 1;
                            }

                        int audio_rate = 22050;
                        /* 22050 é o ideal para a frequência na maioria dos jogos. Se o seu objetivo for criar algum GuitarHero, talvez seja melhor aumentar esse valor. A qualidade de CD é 44100. */

                        Uint16 audio_format = AUDIO_S16SYS; /* Ajuste de amostras com 16 bits. Pode ser necessário alterar para 8 bits: AUDIO_S8 */
                        int audio_channels = 2; /* 1-Mono; 2-Stereo */
                        int audio_buffers = 4096; /* Buffer para armazenamento de trechos do audio*/

                        /* Mix_OpenAudio inicializa o sistema de audio de acordo com as configurações estabelecidas acima */
                        if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
                            fprintf(stderr, "Nao foi possivel inicializar o audio: %s\n", Mix_GetError());
                            exit(1);
                        }

                        /* Ponteiro que receberá a amostra de audio do arquivo WAV e armazenará essa amostra na memória */
                        Mix_Chunk *sound = NULL;

                        sound = Mix_LoadWAV("collect-coin.wav"); /* Carrega a música a partir do arquivo WAV) */
                        if (sound == NULL) {
                            fprintf(stderr, "Impossível carregar arquivo WAV: %s\n", Mix_GetError());
                        }

                        int channel;
                        channel = Mix_PlayChannel(-1, sound, 0);
                        if (channel == -1) {
                            fprintf(stderr, "Impossível reproduzir arquivo WAV: %s\n", Mix_GetError());
                        }
        }
        else
            gameover = true;
    }

    if(dest.x+dest1.w>bola3.x+24&&dest.x<bola3.x+fogo[2].w-28&&dest.y+33<bola3.y+fogo[2].h-23&&dest.y+dest.h>bola3.y+14&&bolafogo3!=NULL){
       if(bar){
            bar = false;
            prot.x = 500;
            prot.y = 550;
            bolafogo3 = NULL;
            ponto+=25;


                            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
                            fprintf(stderr, "Nao foi possivel inicializar o SDL: %s", SDL_GetError());
                            return 1;
                            }

                        int audio_rate = 22050;
                        /* 22050 é o ideal para a frequência na maioria dos jogos. Se o seu objetivo for criar algum GuitarHero, talvez seja melhor aumentar esse valor. A qualidade de CD é 44100. */

                        Uint16 audio_format = AUDIO_S16SYS; /* Ajuste de amostras com 16 bits. Pode ser necessário alterar para 8 bits: AUDIO_S8 */
                        int audio_channels = 2; /* 1-Mono; 2-Stereo */
                        int audio_buffers = 4096; /* Buffer para armazenamento de trechos do audio*/

                        /* Mix_OpenAudio inicializa o sistema de audio de acordo com as configurações estabelecidas acima */
                        if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
                            fprintf(stderr, "Nao foi possivel inicializar o audio: %s\n", Mix_GetError());
                            exit(1);
                        }

                        /* Ponteiro que receberá a amostra de audio do arquivo WAV e armazenará essa amostra na memória */
                        Mix_Chunk *sound = NULL;

                        sound = Mix_LoadWAV("collect-coin.wav"); /* Carrega a música a partir do arquivo WAV) */
                        if (sound == NULL) {
                            fprintf(stderr, "Impossível carregar arquivo WAV: %s\n", Mix_GetError());
                        }

                        int channel;
                        channel = Mix_PlayChannel(-1, sound, 0);
                        if (channel == -1) {
                            fprintf(stderr, "Impossível reproduzir arquivo WAV: %s\n", Mix_GetError());
                        }
        }
        else
            gameover = true;
    }

    if(dest.x+dest1.w>bola4.x+24&&dest.x<bola4.x+fogo[3].w-28&&dest.y+33<bola4.y+fogo[3].h-23&&dest.y+dest.h>bola4.y+14&&bolafogo4!=NULL){
       if(bar){
            bar = false;
            prot.x = 500;
            prot.y = 550;
            bolafogo4 = NULL;
            ponto+=25;


                            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
                            fprintf(stderr, "Nao foi possivel inicializar o SDL: %s", SDL_GetError());
                            return 1;
                            }

                        int audio_rate = 22050;
                        /* 22050 é o ideal para a frequência na maioria dos jogos. Se o seu objetivo for criar algum GuitarHero, talvez seja melhor aumentar esse valor. A qualidade de CD é 44100. */

                        Uint16 audio_format = AUDIO_S16SYS; /* Ajuste de amostras com 16 bits. Pode ser necessário alterar para 8 bits: AUDIO_S8 */
                        int audio_channels = 2; /* 1-Mono; 2-Stereo */
                        int audio_buffers = 4096; /* Buffer para armazenamento de trechos do audio*/

                        /* Mix_OpenAudio inicializa o sistema de audio de acordo com as configurações estabelecidas acima */
                        if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
                            fprintf(stderr, "Nao foi possivel inicializar o audio: %s\n", Mix_GetError());
                            exit(1);
                        }

                        /* Ponteiro que receberá a amostra de audio do arquivo WAV e armazenará essa amostra na memória */
                        Mix_Chunk *sound = NULL;

                        sound = Mix_LoadWAV("collect-coin.wav"); /* Carrega a música a partir do arquivo WAV) */
                        if (sound == NULL) {
                            fprintf(stderr, "Impossível carregar arquivo WAV: %s\n", Mix_GetError());
                        }

                        int channel;
                        channel = Mix_PlayChannel(-1, sound, 0);
                        if (channel == -1) {
                            fprintf(stderr, "Impossível reproduzir arquivo WAV: %s\n", Mix_GetError());
                        }
        }
        else
            gameover = true;
    }

    /**********************************************
    ***********************************************
    ***                                         ***
    ***                 MONSTROS                ***
    ***                                         ***
    ***********************************************
    ***********************************************/

    if(mov1.y+60>215&&mov1.y+60<255&&mov1.x+32>55){
        mov1.x -= 15;
   }
   else  if(mov1.y+60>360&&mov1.y+60<400&&mov1.x+32<890){
        mov1.x += 15;
   }
   else  if(mov1.y+60>500&&mov1.y+60<540&&mov1.x+32>110){
        mov1.x -= 15;
   }
   else  if(mov1.y+60>627&&mov1.y+60<664&&mov1.x+32<970){
        mov1.x += 15;
   }
   else  if(mov1.y+60>747&&mov1.y+60<800&&mov1.x+32>0){
        mov1.x -= 15;

        if(mov1.x<0){
            mov1.x = 900;
            mov1.y = 0;
            monstro = IMG_Load("monstro1.png");
            ponto1 = true;
        }

   }
   else{
        mov1.y += 10;
   }


    if(mov2.y+60>215&&mov2.y+60<255&&mov2.x+32>55){
        mov2.x -= 15;
   }
   else  if(mov2.y+60>360&&mov2.y+60<400&&mov2.x+32<890){
        mov2.x += 15;
   }
   else  if(mov2.y+60>500&&mov2.y+60<540&&mov2.x+32>110){
        mov2.x -= 15;
   }
   else  if(mov2.y+60>627&&mov2.y+60<664&&mov2.x+32<970){
        mov2.x += 15;
   }
   else  if(mov2.y+60>747&&mov2.y+60<800&&mov2.x+32>0){
        mov2.x -= 15;

        if(mov2.x<0){
            mov2.x = 900;
            mov2.y = 0;
            monstro2 = IMG_Load("monstro2.png");
            ponto2 = true;
        }

   }
   else{
        mov2.y += 10;
   }

   if(mov3.y+60>215&&mov3.y+60<255&&mov3.x+32>55){
        mov3.x -= 15;
   }
   else  if(mov3.y+60>360&&mov3.y+60<400&&mov3.x+32<890){
        mov3.x += 15;
   }
   else  if(mov3.y+60>500&&mov3.y+60<540&&mov3.x+32>110){
        mov3.x -= 15;
   }
   else  if(mov3.y+60>627&&mov3.y+60<664&&mov3.x+32<970){
        mov3.x += 15;
   }
   else  if(mov3.y+60>747&&mov3.y+60<800&&mov3.x+32>0){
        mov3.x -= 15;

        if(mov3.x<0){
            mov3.x = 900;
            mov3.y = 0;
            monstro3 = IMG_Load("monstro3.png");
            ponto3 = true;
        }

   }
   else{
        mov3.y += 10;
   }

   if(mov4.y+60>215&&mov4.y+60<255&&mov4.x+32>55){
        mov4.x -= 15;
   }
   else  if(mov4.y+60>360&&mov4.y+60<400&&mov4.x+32<890){
        mov4.x += 15;
   }
   else  if(mov4.y+60>500&&mov4.y+60<540&&mov4.x+32>110){
        mov4.x -= 15;
   }
   else  if(mov4.y+60>627&&mov4.y+60<664&&mov4.x+32<970){
        mov4.x += 15;
   }
   else  if(mov4.y+60>747&&mov4.y+60<800&&mov4.x+32>0){
        mov4.x -= 15;

        if(mov4.x<0){
            mov4.x = 900;
            mov4.y = 0;
            monstro4 = IMG_Load("monstro4.png");
            ponto4 = true;
        }

   }
   else{
        mov4.y += 10;
   }

   mov5.x += 32;
    if(mov5.x > 160)
        mov5.x = 0;

    /**********************************************
    ***********************************************
    ***                                         ***
    ***        COLISAO COM OS MONSTROS          ***
    ***                                         ***
    ***********************************************
    ***********************************************/

    if(dest.x+dest1.w>mov1.x&&dest.x<mov1.x+mov5.w&&dest.y+33<mov1.y+mov5.h&&dest.y+dest.h>mov1.y+30&&monstro!=NULL){
        if(bar){
            bar = false;
            prot.x = 500;
            prot.y = 550;
            monstro = NULL;
        }
        else
            gameover = true;
    }

    if(dest.x+dest1.w>mov2.x&&dest.x<mov2.x+mov5.w&&dest.y+33<mov2.y+mov5.h&&dest.y+dest.h>mov2.y+30&&monstro2!=NULL){
        if(bar){
            bar = false;
            prot.x = 500;
            prot.y = 550;
            monstro2 = NULL;
        }
        else
            gameover = true;
    }

    if(dest.x+dest1.w>mov3.x&&dest.x<mov3.x+mov5.w&&dest.y+33<mov3.y+mov5.h&&dest.y+dest.h>mov3.y+30&&monstro3!=NULL){
        if(bar){
            bar = false;
            prot.x = 500;
            prot.y = 550;
            monstro3 = NULL;
        }
        else
            gameover = true;
    }

    if(dest.x+dest1.w>mov4.x&&dest.x<mov4.x+mov5.w&&dest.y+33<mov4.y+mov5.h&&dest.y+dest.h>mov4.y+30&&monstro4!=NULL){
       if(bar){
            bar = false;
            prot.x = 500;
            prot.y = 550;
            monstro4 = NULL;
        }
        else
            gameover = true;
    }


    /**********************************************
    ***********************************************
    ***                                         ***
    ***                   BOSS                  ***
    ***                                         ***
    ***********************************************
    ***********************************************/

        if(tempo==1&&b1.x!=750){
            tempo = 0;
            b1.x += 150;
        }
        else if(tempo == 16)
        {
            b1.x = 0;
            tempo = 0;
        }
        else
            tempo++;



    /**********************************************
    ***********************************************
    ***                                         ***
    ***            COMANDOS DA LAVA             ***
    ***                                         ***
    ***********************************************
    ***********************************************/

        p1.y--;

        if(p1.y<dest.y+50)
        {
            gameover = true;

        }

    /**********************************************
    ***********************************************
    ***                                         ***
    ***                 BARREIRA                ***
    ***                                         ***
    ***********************************************
    ***********************************************/

    if(dest.x+dest1.w>prot.x&&dest.x<prot.x+50&&dest.y<prot.y+50&&dest.y+dest.h>prot.y)
    {
        prot.x = dest.x;
        prot.y = dest.y+25;
        bar = true;
    }
    else
        bar = false;


    /**********************************************
    ***********************************************
    ***                                         ***
    ***           CONCLUINDO O JOGO!            ***
    ***                                         ***
    ***********************************************
    ***********************************************/


        if(dest.y>100&&dest.y<180&&dest.x>860&&plataforma){
            if(bar){
                ponto += 100;

                int audio_rate = 22050;
                        /* 22050 é o ideal para a frequência na maioria dos jogos. Se o seu objetivo for criar algum GuitarHero, talvez seja melhor aumentar esse valor. A qualidade de CD é 44100. */

                        Uint16 audio_format = AUDIO_S16SYS; /* Ajuste de amostras com 16 bits. Pode ser necessário alterar para 8 bits: AUDIO_S8 */
                        int audio_channels = 2; /* 1-Mono; 2-Stereo */
                        int audio_buffers = 4096; /* Buffer para armazenamento de trechos do audio*/

                        /* Mix_OpenAudio inicializa o sistema de audio de acordo com as configurações estabelecidas acima */
                        if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
                            fprintf(stderr, "Nao foi possivel inicializar o audio: %s\n", Mix_GetError());
                            exit(1);
                        }

                        /* Ponteiro que receberá a amostra de audio do arquivo WAV e armazenará essa amostra na memória */
                        Mix_Chunk *sound = NULL;

                        sound = Mix_LoadWAV("collect-coin.wav"); /* Carrega a música a partir do arquivo WAV) */
                        if (sound == NULL) {
                            fprintf(stderr, "Impossível carregar arquivo WAV: %s\n", Mix_GetError());
                        }

                        int channel;
                        channel = Mix_PlayChannel(-1, sound, 0);
                        if (channel == -1) {
                            fprintf(stderr, "Impossível reproduzir arquivo WAV: %s\n", Mix_GetError());
                        }

                }
            concluido = true;
        }

    /**********************************************
    ***********************************************
    ***                                         ***
    ***                PONTUAÇÃO                ***
    ***                                         ***
    ***********************************************
    ***********************************************/

    ponto--;
    score[0] = '0' + ponto/10000;
    score[1] = '0' + (ponto%10000)/1000;
    score[2] = '0' + ((ponto%10000)%1000)/100;
    score[3] = '0' + (((ponto%10000)%1000)%100)/10;
    score[4] = '0' + (((ponto%10000)%1000)%100)%10;
    if(ponto<=0)
        ponto = 0;

    if(dest.x+dest1.w>mov1.x&&dest.x<mov1.x+mov5.w&&dest.y+dest1.h<mov1.y+30&&dest.y+dest1.h>mov1.y-100&&monstro!=NULL&&ponto1){
       if(bar)
            ponto += 50;
       else
            ponto += 25;
       ponto1 = false;

       if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
                            fprintf(stderr, "Nao foi possivel inicializar o SDL: %s", SDL_GetError());
                            return 1;
                            }

                        int audio_rate = 22050;
                        /* 22050 é o ideal para a frequência na maioria dos jogos. Se o seu objetivo for criar algum GuitarHero, talvez seja melhor aumentar esse valor. A qualidade de CD é 44100. */

                        Uint16 audio_format = AUDIO_S16SYS; /* Ajuste de amostras com 16 bits. Pode ser necessário alterar para 8 bits: AUDIO_S8 */
                        int audio_channels = 2; /* 1-Mono; 2-Stereo */
                        int audio_buffers = 4096; /* Buffer para armazenamento de trechos do audio*/

                        /* Mix_OpenAudio inicializa o sistema de audio de acordo com as configurações estabelecidas acima */
                        if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
                            fprintf(stderr, "Nao foi possivel inicializar o audio: %s\n", Mix_GetError());
                            exit(1);
                        }

                        /* Ponteiro que receberá a amostra de audio do arquivo WAV e armazenará essa amostra na memória */
                        Mix_Chunk *sound = NULL;

                        sound = Mix_LoadWAV("collect-coin.wav"); /* Carrega a música a partir do arquivo WAV) */
                        if (sound == NULL) {
                            fprintf(stderr, "Impossível carregar arquivo WAV: %s\n", Mix_GetError());
                        }

                        int channel;
                        channel = Mix_PlayChannel(-1, sound, 0);
                        if (channel == -1) {
                            fprintf(stderr, "Impossível reproduzir arquivo WAV: %s\n", Mix_GetError());
                        }
    }

    if(dest.x+dest1.w>mov2.x&&dest.x<mov2.x+mov5.w&&dest.y+dest1.h<mov2.y+30&&dest.y+dest1.h>mov2.y-100&&monstro2!=NULL&&ponto2){
         if(bar)
            ponto += 50;
       else
            ponto += 25;
        ponto2 = false;

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
                            fprintf(stderr, "Nao foi possivel inicializar o SDL: %s", SDL_GetError());
                            return 1;
                            }

                        int audio_rate = 22050;
                        /* 22050 é o ideal para a frequência na maioria dos jogos. Se o seu objetivo for criar algum GuitarHero, talvez seja melhor aumentar esse valor. A qualidade de CD é 44100. */

                        Uint16 audio_format = AUDIO_S16SYS; /* Ajuste de amostras com 16 bits. Pode ser necessário alterar para 8 bits: AUDIO_S8 */
                        int audio_channels = 2; /* 1-Mono; 2-Stereo */
                        int audio_buffers = 4096; /* Buffer para armazenamento de trechos do audio*/

                        /* Mix_OpenAudio inicializa o sistema de audio de acordo com as configurações estabelecidas acima */
                        if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
                            fprintf(stderr, "Nao foi possivel inicializar o audio: %s\n", Mix_GetError());
                            exit(1);
                        }

                        /* Ponteiro que receberá a amostra de audio do arquivo WAV e armazenará essa amostra na memória */
                        Mix_Chunk *sound = NULL;

                        sound = Mix_LoadWAV("collect-coin.wav"); /* Carrega a música a partir do arquivo WAV) */
                        if (sound == NULL) {
                            fprintf(stderr, "Impossível carregar arquivo WAV: %s\n", Mix_GetError());
                        }

                        int channel;
                        channel = Mix_PlayChannel(-1, sound, 0);
                        if (channel == -1) {
                            fprintf(stderr, "Impossível reproduzir arquivo WAV: %s\n", Mix_GetError());
                        }
    }

    if(dest.x+dest1.w>mov3.x&&dest.x<mov3.x+mov5.w&&dest.y+dest1.h<mov3.y+30&&dest.y+dest1.h>mov3.y-100&&monstro3!=NULL&&ponto3){
         if(bar)
            ponto += 50;
       else
            ponto += 25;

        ponto3 = false;

                    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
                    fprintf(stderr, "Nao foi possivel inicializar o SDL: %s", SDL_GetError());
                    return 1;
                            }

                        int audio_rate = 22050;
                        /* 22050 é o ideal para a frequência na maioria dos jogos. Se o seu objetivo for criar algum GuitarHero, talvez seja melhor aumentar esse valor. A qualidade de CD é 44100. */

                        Uint16 audio_format = AUDIO_S16SYS; /* Ajuste de amostras com 16 bits. Pode ser necessário alterar para 8 bits: AUDIO_S8 */
                        int audio_channels = 2; /* 1-Mono; 2-Stereo */
                        int audio_buffers = 4096; /* Buffer para armazenamento de trechos do audio*/

                        /* Mix_OpenAudio inicializa o sistema de audio de acordo com as configurações estabelecidas acima */
                        if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
                            fprintf(stderr, "Nao foi possivel inicializar o audio: %s\n", Mix_GetError());
                            exit(1);
                        }

                        /* Ponteiro que receberá a amostra de audio do arquivo WAV e armazenará essa amostra na memória */
                        Mix_Chunk *sound = NULL;

                        sound = Mix_LoadWAV("collect-coin.wav"); /* Carrega a música a partir do arquivo WAV) */
                        if (sound == NULL) {
                            fprintf(stderr, "Impossível carregar arquivo WAV: %s\n", Mix_GetError());
                        }

                        int channel;
                        channel = Mix_PlayChannel(-1, sound, 0);
                        if (channel == -1) {
                            fprintf(stderr, "Impossível reproduzir arquivo WAV: %s\n", Mix_GetError());
                        }
    }

    if(dest.x+dest1.w>mov4.x&&dest.x<mov4.x+mov5.w&&dest.y+dest1.h<mov4.y+30&&dest.y+dest1.h>mov4.y-100&&monstro4!=NULL&&ponto4){
        if(bar)
            ponto += 50;
       else
            ponto += 25;

    //Musica

                            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
                            fprintf(stderr, "Nao foi possivel inicializar o SDL: %s", SDL_GetError());
                            return 1;
                            }

                        int audio_rate = 22050;
                        /* 22050 é o ideal para a frequência na maioria dos jogos. Se o seu objetivo for criar algum GuitarHero, talvez seja melhor aumentar esse valor. A qualidade de CD é 44100. */

                        Uint16 audio_format = AUDIO_S16SYS; /* Ajuste de amostras com 16 bits. Pode ser necessário alterar para 8 bits: AUDIO_S8 */
                        int audio_channels = 2; /* 1-Mono; 2-Stereo */
                        int audio_buffers = 4096; /* Buffer para armazenamento de trechos do audio*/

                        /* Mix_OpenAudio inicializa o sistema de audio de acordo com as configurações estabelecidas acima */
                        if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
                            fprintf(stderr, "Nao foi possivel inicializar o audio: %s\n", Mix_GetError());
                            exit(1);
                        }

                        /* Ponteiro que receberá a amostra de audio do arquivo WAV e armazenará essa amostra na memória */
                        Mix_Chunk *sound = NULL;

                        sound = Mix_LoadWAV("collect-coin.wav"); /* Carrega a música a partir do arquivo WAV) */
                        if (sound == NULL) {
                            fprintf(stderr, "Impossível carregar arquivo WAV: %s\n", Mix_GetError());
                        }

                        int channel;
                        channel = Mix_PlayChannel(-1, sound, 0);
                        if (channel == -1) {
                            fprintf(stderr, "Impossível reproduzir arquivo WAV: %s\n", Mix_GetError());
                        }


        ponto4 = false;
    }

     }

      else if(gameover){
        risada = IMG_Load("risada.png");
        princesa = IMG_Load("gameover.png");
        dest1.y = 525;
        dest1.x = 0;
        dest1.h--;
        if(dest1.h<25)
            tela = IMG_Load("over.png");
        if(dest1.h<=0)
        {
            imagem = NULL;
            ponto = 0;
            score[0] = '0' + ponto/10000;
            score[1] = '0' + (ponto%10000)/1000;
            score[2] = '0' + ((ponto%10000)%1000)/100;
            score[3] = '0' + (((ponto%10000)%1000)%100)/10;
            score[4] = '0' + (((ponto%10000)%1000)%100)%10;
            dest1.x=0;
            dest1.y=0;
            dest1.w=382;
            dest1.h=75;
            dest.x  =  350;
            dest.y  =  350;
            p1.y = 900;
            auY = 0;
            bolafogo = NULL;
            bola.y = 0;
            bolafogo2 = NULL;
            bola2.y = 0;
            bolafogo3 = NULL;
            bola3.y = 0;
            bolafogo4 = NULL;
            bola4.y = 0;
            BOSS = NULL;
            barreira = NULL;
            gameover = false;
            fase = 0;
            risada = NULL;
            fundo = IMG_Load("menu.png");
            imagem = SDL_LoadBMP("semibotao.bmp");
            SDL_SetColorKey(imagem, SDL_SRCCOLORKEY, SDL_MapRGB(imagem->format, 255, 255, 255));
            tela = NULL;
        }
    }
        else{
            princesa = IMG_Load("parabens.png");
            if(ponto>=r1)
                tela = IMG_Load("recordewin.png");
            else
                tela = IMG_Load("win.png");
            dest1.y = 0;
            dest1.x = 0;
            monstro = NULL;
            monstro2 = NULL;
            monstro3 = NULL;
            monstro4 = NULL;
            bolafogo = NULL;
            bolafogo2 = NULL;
            bolafogo3 = NULL;
            bolafogo4 = NULL;
            barreira = NULL;
            b1.h-=3;
            if(ponto>r1){
                r1 = ponto;
                recorde = fopen("recorde.txt","w");
                fprintf(recorde,"%d",r1);
                fclose(recorde);
            }
            if(b1.h <= 10){
                imagem = NULL;
                dest1.x=0;
                dest1.y=0;
                dest1.w=382;
                dest1.h=75;
                dest.x  =  350;
                dest.y  =  350;
                p1.y = 900;
                auY = 0;
                bolafogo = NULL;
                bola.y = 0;
                bolafogo2 = NULL;
                bola2.y = 0;
                bolafogo3 = NULL;
                bola3.y = 0;
                bolafogo4 = NULL;
                bola4.y = 0;
                BOSS = NULL;
                concluido = false;
                fase = 0;
            fundo = IMG_Load("menu.png");
            imagem = SDL_LoadBMP("semibotao.bmp");
            SDL_SetColorKey(imagem, SDL_SRCCOLORKEY, SDL_MapRGB(imagem->format, 255, 255, 255));
            tela = NULL;

            }

        }
    }




    /**********************************************
    ***********************************************
    ***                                         ***
    ***       COLOCANDO A IMAGEM NA TELA        ***
    ***                                         ***
    ***********************************************
    ***********************************************/

    SDL_Color cor = {255, 255, 255}; //vermelho
    SDL_Surface* src = TTF_RenderText_Solid(my_font, score , cor);
    if(fase == 1)
        src2 = TTF_RenderText_Solid(my_font, "score:" , cor);
    else{
        src2 = TTF_RenderText_Solid(my_font, "último score:" , cor);
        hsrc1 = TTF_RenderText_Solid(my_font, "Melhor score:" , cor);
        hsrc2 = TTF_RenderText_Solid(my_font, hscore , cor);
        }

    SDL_Rect scr3 = {0,20,0,0};
    SDL_Rect scr4 = {0,40,0,0};
    SDL_Rect scr5 = {0,60,0,0};

    SDL_FillRect(screen, NULL, 0x0);
    SDL_BlitSurface(fundo,NULL,screen,NULL);
    SDL_BlitSurface(BOSS,&b1,screen,&b2);
    SDL_BlitSurface(risada,NULL,screen,&b3);
    SDL_BlitSurface(imagem, &dest1, screen, &dest);
    SDL_BlitSurface(barreira,NULL,screen, &prot);
    SDL_BlitSurface(princesa,NULL,screen,&prince);
    SDL_BlitSurface(monstro, &mov5, screen, &mov1);
    SDL_BlitSurface(monstro2, &mov5, screen, &mov2);
    SDL_BlitSurface(monstro3, &mov5, screen, &mov3);
    SDL_BlitSurface(monstro4, &mov5, screen, &mov4);
    SDL_BlitSurface(bolafogo,&fogo[0],screen,&bola);
    SDL_BlitSurface(bolafogo2,&fogo[1],screen,&bola2);
    SDL_BlitSurface(bolafogo3,&fogo[2],screen,&bola3);
    SDL_BlitSurface(bolafogo4,&fogo[3],screen,&bola4);
    SDL_BlitSurface(src2, NULL, screen, NULL);
    SDL_BlitSurface(src, NULL, screen, &scr3);

    if(fase != 1){
    SDL_BlitSurface(hsrc1, NULL, screen, &scr4);
    SDL_BlitSurface(hsrc2, NULL, screen, &scr5);
    }

    if(fase==1)
        SDL_BlitSurface(lava,NULL,screen,&p1);

    SDL_BlitSurface(tela,NULL,screen,NULL);



    /**********************************************
    ***********************************************
    ***                                         ***
    ***             CONTROLE DE FPS             ***
    ***                                         ***
    ***********************************************
    ***********************************************/

    if(1000/FPS >(SDL_GetTicks()-start))
          SDL_Delay(1000/FPS-(SDL_GetTicks()-start));
       SDL_Flip( screen);


  }



    SDL_Quit();


   return 0;
}
