#include "HelloWorldScene.h"
#include "DJZipArchive.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    //解压文件测试
    auto destName = FileUtils::getInstance()->getWritablePath();
    
    unZipFiles(destName.c_str(), "Resources.zip");
    
    return true;
}

//unZipFiles 参数是解压的目的目录和源目录
bool HelloWorld::unZipFiles(const char *destName, const char *srcName)
{
	DJZipArchive * djzip = new DJZipArchive();
    if (!djzip->unzipOpenFile(srcName))
    {
        log("cant't open %s",srcName);
    }
    
    //可以解压带目录的 带密码的
    return djzip->unzipFileToAndOverWrite(destName,true);
}
