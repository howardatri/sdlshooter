#include "Game.h"
#include <SDL.h>
#include "SceneMain.h"
#include <SDL_image.h>

Game::Game()
{
}

Game::~Game()
{
}
void Game::init(){
    frameTime=1000/FPS;

    //初始化SDL
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"SDL_Init Error: %s\n",SDL_GetError());
        isRunning=false;
    }
    //创建窗口
    window = SDL_CreateWindow("SDL",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,windowWidth,windowHeight,0);
    if(window==nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"SDL_CreateWindow Error: %s\n",SDL_GetError());
        isRunning=false;
    }
    //创建渲染器
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    if(renderer==nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"SDL_CreateRenderer Error: %s\n",SDL_GetError());
        isRunning=false;
    }
    //初始化图片
    if(IMG_Init(IMG_INIT_PNG)!=IMG_INIT_PNG){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"IMG_Init Error: %s\n",IMG_GetError());
        isRunning=false;
    }

    currentScene=new SceneMain();
    currentScene->init();
    

}
void Game::run(){
    while(isRunning){
        auto frameStart=SDL_GetTicks();

        SDL_Event event;
        handleEvent(&event);

        
        update(deltaTime);
        render();

        auto frameEnd=SDL_GetTicks();
        
        auto diff=frameEnd-frameStart;
        if(diff<frameTime){
            SDL_Delay(frameTime-diff);
            deltaTime=frameTime/1000.0f;
        }
        else{
            deltaTime=diff/1000.0f;
        }
    }

}
void Game::clean(){
    if(currentScene!=nullptr){
        currentScene->clean();
        delete currentScene;
    }

    //清理SDL_image
    IMG_Quit();
 

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::changeScene(Scene *scene)
{
    if(currentScene!=nullptr){
        currentScene->clean();
        delete currentScene;
    }
    currentScene=scene;
    currentScene->init();
}

void Game::handleEvent(SDL_Event *event)
{
    while(SDL_PollEvent(event)){//退出机制非常重要
        if(event->type == SDL_QUIT){
            isRunning=false;
        }
        currentScene->handleEvent(event);
    }
}

void Game::update(float deltaTime)
{
    currentScene->update(deltaTime);
}

void Game::render()
{
        //清空
        SDL_RenderClear(renderer);
        currentScene->render();
        //显示更新
        SDL_RenderPresent(renderer);
}
