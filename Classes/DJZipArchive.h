//
//  DJZipArchive.h
//  TestUI
//
//  Created by �ż� on 14-5-3.
//
//

#ifndef __TestUI__DJZipArchive__
#define __TestUI__DJZipArchive__

#include "cocos2d.h"
#include "unzip/unzip.h"

USING_NS_CC;


class DJZipArchive {
    
public:
    DJZipArchive();
    ~DJZipArchive();
    
    bool unzipOpenFile(const char* zipFile);
    
    bool unzipFileToAndOverWrite(std::string path,bool overwrite);
    
    bool createDirectory(const char *path);
    
    void setPassword(std::string psw){_password = psw;};
   
private:
    unzFile _unzFile;
    std::string _password;   
};

#endif /* defined(__TestUI__DJZipArchive__) */
