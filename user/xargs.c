#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

#define MAXLINE 512  // xv6 中使用较小的缓冲区

// 读取一行，返回读取的字节数
int readline(int fd, char *buf, int size)
{
    int i = 0;
    char c;
    while(i < size-1){
        if(read(fd, &c, 1) != 1){
            break;
        }
        if(c == '\n'){
            break;
        }
        buf[i++] = c;
    }
    buf[i] = '\0';
    return i;
}

int main(int argc, char *argv[])
{
    if(argc < 2){
        fprintf(2, "Usage: xargs [-n max-args] command [initial-arguments]\n");
        exit(1);
    }

    char buf[MAXLINE];
    char *cmd[MAXARG];
    int max_args = -1;  // 默认不限制参数数量
    int cmd_start = 1;

    // 处理-n参数
    if(strcmp(argv[1], "-n") == 0){
        if(argc < 4){
            fprintf(2, "xargs: missing argument to -n\n");
            exit(1);
        }
        max_args = atoi(argv[2]);
        if(max_args <= 0 || max_args >= MAXARG){
            fprintf(2, "xargs: invalid number for -n\n");
            exit(1);
        }
        cmd_start = 3;
    }

    // 准备基础命令参数
    int base_args = 0;
    for(int i = cmd_start; i < argc && base_args < MAXARG-1; i++){
        cmd[base_args++] = argv[i];
    }
    cmd[base_args] = 0;

    // 处理标准输入
    while(readline(0, buf, sizeof(buf)) > 0){
        int pid = fork();
        if(pid < 0){
            fprintf(2, "xargs: fork failed\n");
            exit(1);
        }
        if(pid == 0){
            char *args[MAXARG];
            int i;
            
            // 复制基础参数
            for(i = 0; i < base_args && i < MAXARG-1; i++){
                args[i] = cmd[i];
            }

            // 添加新参数
            char *token = buf;
            int arg_count = 0;

            while(token && *token && i < MAXARG-1 && 
                  (max_args == -1 || arg_count < max_args)){
                // 跳过空格
                while(*token == ' ') token++;
                if(!*token) break;

                // 找到下一个空格
                char *end = token;
                while(*end && *end != ' ') end++;
                if(*end) *end++ = '\0';

                if(*token){
                    args[i++] = token;
                    arg_count++;
                }
                token = end;
            }
            args[i] = 0;

            // xv6中直接执行命令
            exec(args[0], args);
            fprintf(2, "xargs: exec %s failed\n", args[0]);
            exit(1);
        } else {
            wait(0);
        }
    }
    exit(0);
}