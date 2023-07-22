#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/stat.h>
#include "safe_call.h"

static void read_package_name(char *packageName,size_t max_len){
    if(packageName == NULL){
        return;
    }
    char cmdline_path[128] = {0};
    snprintf(cmdline_path,128,"/proc/%d/cmdline",getpid());
    FILE *fp = fopen(cmdline_path,"rb");
    if(NULL == fp){
        return;
    }
    fgets(packageName,max_len,fp);
    fclose(fp);
    size_t real_len = strnlen(packageName,max_len);
    for(size_t i = 0;i < real_len;i++){
        if(packageName[i] == ':'){
            packageName[i] = '\0';
            break;
        }
    }
}

static void get_workspace_dir(char *workspace_dir,size_t max_len){
    char package_name[256] = {0};
    read_package_name(package_name,256);

    snprintf(workspace_dir,max_len,"/data/data/%s/code_cache",package_name);
}

int before_call(const void *func_addr) {

    Dl_info dl_info;
    int ret = dladdr(func_addr, &dl_info);
    if(ret != 0) {
        char workspace_dir[1024] = {0};
        get_workspace_dir(workspace_dir,1024);
        if(access(workspace_dir,F_OK) != 0){
            int ok = mkdir(workspace_dir,0755) == 0;
            if(!ok) {
                return 1;
            }
        }
        uintptr_t local_func_addr = ((uintptr_t)func_addr - (uintptr_t)dl_info.dli_fbase);
        char record_file_name[1024] = {0};
        snprintf(record_file_name,1024,"%s/sc_%lx",workspace_dir,local_func_addr);
        if(access(record_file_name,F_OK) != 0) {
            FILE *fp = fopen(record_file_name,"w");
            if(fp) {
                fclose(fp);
            }

            return 1;
        }
        else {
            return 0;
        }
    }
    return 1;
}

void after_call(const void *func_addr) {
    Dl_info info;
    int ret = dladdr(func_addr, &info);
    if(ret != 0) {
        char workspace_dir[1024] = {0};
        get_workspace_dir(workspace_dir,1024);
        if(access(workspace_dir,F_OK) != 0){
            return;
        }
        uintptr_t local_func_addr = ((uintptr_t)func_addr - (uintptr_t)info.dli_fbase);
        char record_file_name[1024] = {0};
        snprintf(record_file_name,1024,"%s/sc_%lx",workspace_dir,local_func_addr);
        if(access(record_file_name,F_OK) == 0) {
            remove(record_file_name);
        }

    }
}
