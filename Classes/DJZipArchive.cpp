//
//  DJZipArchive.cpp
//  TestUI
//
//  Created by �ż� on 14-5-3.
//
//

#include "DJZipArchive.h"
#include "stdio.h"
//使用mkdir函数
#include "sys/stat.h"

DJZipArchive::DJZipArchive()
{
    _unzFile=  nullptr;
}

//打开要解压的文件
bool DJZipArchive::unzipOpenFile(const char *zipFile)
{ 
    _unzFile = cocos2d::unzOpen(zipFile);
    if (_unzFile)
    {
//        typedef struct unz_global_info_s
//        {
//            uLong number_entry;         /* total number of entries in
//                                         the central dir on this disk */
//            uLong size_comment;         /* size of the global comment of the zipfile */
//        } unz_global_info;
        unz_global_info globalInfo = {0};
        if (unzGetGlobalInfo(_unzFile, &globalInfo) == UNZ_OK)
        {
            //文件个数
            log("%lu  entries in the zip file",globalInfo.number_entry);
        }
    }
    return _unzFile != nullptr;
}

//解压文件
bool DJZipArchive::unzipFileToAndOverWrite(std::string path, bool overwrite)
{
    bool success = true;
    //当前的文件为第一个要解压的文件
    int ret = unzGoToFirstFile(_unzFile);
    
    if(ret != UNZ_OK)
        return false;
    
    unsigned char buffer[4096] = {0};

    do
	{
        //判断是否存在密码，打开当前要解压的文件
        if (_password.size() == 0)
            ret = unzOpenCurrentFile(_unzFile);
        else
            ret = unzOpenCurrentFilePassword(_unzFile, _password.c_str());
        
        if (ret != UNZ_OK) 
		{
            success = false;
            break;
        }
        
        //fileInfo中存放当前要解压文件的信息
        int read ;
        unz_file_info fileInfo = {0};
        ret = unzGetCurrentFileInfo(_unzFile, &fileInfo, nullptr, 0, nullptr, 0, nullptr, 0);
        if (ret != UNZ_OK) 
		{
            success = false;
            //关闭当前要解压的文件
            unzCloseCurrentFile(_unzFile);
        }
        
        //获得当前的文件名
        char* filename = (char*)malloc(fileInfo.size_filename + 1);
        unzGetCurrentFileInfo(_unzFile, &fileInfo, filename, fileInfo.size_filename + 1, nullptr, 0, nullptr, 0);
        filename[fileInfo.size_filename] = '\0';
        log("filename = %s",filename);
        
        //判断当前的文件名是否是目录
        bool isDirectory = false;
        if (filename[fileInfo.size_filename - 1] == '/' || filename[fileInfo.size_filename - 1] == '\\')
            isDirectory = true;
        
        std::string fullPath = path + StringUtils::format("%s",filename);
        
        if (isDirectory)
		{
            this->createDirectory(fullPath.c_str());
        }

        //开始写入数据
        FILE* fp = fopen(fullPath.c_str(), "wb");
        while (fp) 
		{
            read = unzReadCurrentFile(_unzFile, buffer, 4096);
            if (read > 0) {
                fwrite(buffer, read, 1, fp);
            }
            else if (read < 0)
            {
                break;
            }
            else
                break;
            
        }
        
        if (fp) 
		{
            fclose(fp);
        }

        //关闭当前的文件，解压下一个文件
        unzCloseCurrentFile(_unzFile);
        ret = unzGoToNextFile(_unzFile);
    } while (ret == UNZ_OK && ret != UNZ_END_OF_LIST_OF_FILE);
    
	return success;
}

bool DJZipArchive::createDirectory(const char *path)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0)
    {
        return false;
    }
    
    return true;
#else
    BOOL ret = CreateDirectoryA(path, NULL);
	if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
	{
		return false;
	}
    return true;
#endif
}
