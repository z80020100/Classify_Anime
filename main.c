#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/*
 * 作者：Cliff Wu
 * 功能：自動將動畫、影集依以下規則分類放入資料夾
 * 判斷規則：集數前的字串當作資料夾名稱
 * 範例：
 *        "[SumiSora][Sword_Art_Online][01][GB][720p].mp4"
 *        自動產生資料夾"[SumiSora][Sword_Art_Online]"，並將上述檔案放入
 * 使用方法：將執行檔放入欲整理的目錄並執行，結果會顯示在螢幕並紀錄在"log.txt"
 * 注意：該目錄底下請不要放置其他檔案或資料夾！
 * History:
 *          2015.1.31   First Release
 *          2015.2.1    排除檔名一開始就為數字的檔案，避免程式讀到空的檔案名稱
 *          2015.2.5    修正上述相同問題並新增無法分類資料，放入開頭即為數字的檔案
 */

int main(void)
{
    DIR *dp, *dp2;
    FILE *fPtr;
    struct dirent *ep, *ep2;
    char command[256];
    char name[sizeof(ep->d_name)];
    int i, file, find, state = 0; // 利用state來判斷是否有進行整理
    time_t rawtime;
    struct tm *timeinfo;
    char systemTime[64];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    puts("動畫分類程式V2015.2.5 by Cliff Wu");
    puts("*********************************");
    puts("用途：自動將動畫、影集依以下規則分類放入資料夾");
    puts("判斷規則：集數前的字串當作資料夾名稱");
    puts("範例：");
    puts("       \"[SumiSora][Sword_Art_Online][01][GB][720p].mp4\"");
    puts("       自動產生資料夾\"[SumiSora][Sword_Art_Online]\"，並將上述檔案放入");
    puts("使用方法：將執行檔放入欲整理的目錄並執行，結果會顯示在螢幕並紀錄在\"log.txt\"");
    puts("注意：該目錄底下請不要放置其他檔案或資料夾！");
    puts("！本程式僅供分類檔案使用，若有任何損失恕不負責！");
    puts("*******************************");
    puts("同意請按任意鍵開始執行或按X離開");
    system("pause>nul");
    puts("");

    // 顯示並紀錄系統時間
    strftime(systemTime, sizeof(systemTime), "%Y-%m-%d %H:%M:%S", timeinfo);
    printf("----------------%s----------------\n", systemTime);

    fPtr = fopen("log.txt", "a+");
    fprintf(fPtr, "----------------%s----------------\n", systemTime);

    dp = opendir("./");
    if(dp != NULL)
    {
        while(ep = readdir(dp))
        {
            memset(name, 0, sizeof(name));
            memset(command, 0, sizeof(command));

            // 使用檔名中的'.'來做簡易判斷是檔案還是資料夾
            file = 0;
            for(i = 0; i < (ep->d_namlen); i++)
            {
                if(ep->d_name[i] == '.')
                    file = 1;
            }
            /* 印出目錄底下所有資料夾及檔案
            if(file == 1)
                printf("File: %s\n", ep->d_name); // file name
            else
                printf("Dir: %s\n", ep->d_name); // dir name
            */

            for(i = 1; i < (ep->d_namlen)-1; i++) // d_namlen: length of file name
            {
                //temp[0] = ep->d_name[i];
                //temp[1] = ep->d_name[i+1];
                //printf("%s\n", temp);
                if((ep->d_name[i])-'0' >= 0 && (ep->d_name[i])-'0' <= 9)
                {
                    if((ep->d_name[i+1])-'0' >= 0 && (ep->d_name[i+1])-'0' <= 9)
                    {
                        state = 1; //進入整理程序
                        // 找到最可能是集數的兩個字元
                        if(i-1 != 0)
                        {
                            strncpy(name, (ep->d_name), i-1); // 以這兩個字元前的字串當作資料夾名稱
                            //printf("%s\n", name);
                        }
                        else
                            strcpy(name, "unclassifiable");

                        // 搜尋是否資料夾已建立
                        dp2 = opendir("./");
                        if(dp2 != NULL)
                        {
                            find = 0;
                            while(ep2 = readdir(dp2))
                            {
                                //printf("%s\n", ep2->d_name);
                                if(strcmp(ep2->d_name, name) == 0)
                                {
                                    find = 1;
                                }
                            }
                            if(find == 1) // 資料夾已存在
                            {
                                printf("\nFile: %s\n", ep->d_name); // file name
                                printf("    Find folder \"%s\"\n", name);
                                printf("    Move \"%s\" to \"%s\"\n",ep->d_name, name);
                                fprintf(fPtr, "\nFile: %s\n", ep->d_name); // file name
                                fprintf(fPtr, "    Find folder \"%s\"\n", name);
                                fprintf(fPtr, "    Move \"%s\" to \"%s\"\n",ep->d_name, name);
                                sprintf(command, "move \"%s\" \"%s\"\\", ep->d_name, name);
                                /*
                                    strcat(command, "move ");
                                    strcat(command, "\"");
                                    strcat(command, ep->d_name);
                                    strcat(command, "\" ");
                                    strcat(command, "\"");
                                    strcat(command, name);
                                    strcat(command, "\"\\");
                                */
                                //printf("%s\n", command);
                                system(command);
                            }
                            else // 資料夾不存在
                            {
                                printf("\nFile: %s\n", ep->d_name); // file name
                                printf("    Create a folder \"%s\"\n", name);
                                fprintf(fPtr, "\nFile: %s\n", ep->d_name); // file name
                                fprintf(fPtr, "    Crete a folder \"%s\"\n", name);

                                sprintf(command, "mkdir \"%s\"", name);
                                //printf("%s\n", command);
                                system(command);

                                printf("    Move \"%s\" to \"%s\"\n",ep->d_name, name);
                                fprintf(fPtr, "    Move \"%s\" to \"%s\"\n",ep->d_name, name);

                                sprintf(command, "move \"%s\" \"%s\"\\", ep->d_name, name);
                                //printf("%s\n", command);
                                system(command);
                            }
                        }
                        (void)closedir(dp2);
                        break;
                    }
                }
            }
        }
        (void)closedir(dp);
    }
    else
        perror("Couldn't open the directory");

    if(state == 0)
    {
        printf("檔案都已整理完畢\n");
        fprintf(fPtr, "All files have been classified.\n");
    }

    printf("------------------------End------------------------\n");
    fprintf(fPtr, "------------------------End------------------------\n");
    fclose(fPtr);

    printf("處理完畢，請按任意鍵結束");
    system("pause>nul");
    return 0;
}
