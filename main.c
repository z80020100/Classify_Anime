#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/*
 * �@�̡GCliff Wu
 * �\��G�۰ʱN�ʵe�B�v���̥H�U�W�h������J��Ƨ�
 * �P�_�W�h�G���ƫe���r���@��Ƨ��W��
 * �d�ҡG
 *        "[SumiSora][Sword_Art_Online][01][GB][720p].mp4"
 *        �۰ʲ��͸�Ƨ�"[SumiSora][Sword_Art_Online]"�A�ñN�W�z�ɮש�J
 * �ϥΤ�k�G�N�����ɩ�J����z���ؿ��ð���A���G�|��ܦb�ù��ì����b"log.txt"
 * �`�N�G�ӥؿ����U�Ф��n��m��L�ɮשθ�Ƨ��I
 * History:
 *          2015.1.31   First Release
 *          2015.2.1    �ư��ɦW�@�}�l�N���Ʀr���ɮסA�קK�{��Ū��Ū��ɮצW��
 *          2015.2.5    �ץ��W�z�ۦP���D�÷s�W�L�k������ơA��J�}�Y�Y���Ʀr���ɮ�
 */

int main(void)
{
    DIR *dp, *dp2;
    FILE *fPtr;
    struct dirent *ep, *ep2;
    char command[256];
    char name[sizeof(ep->d_name)];
    int i, file, find, state = 0; // �Q��state�ӧP�_�O�_���i���z
    time_t rawtime;
    struct tm *timeinfo;
    char systemTime[64];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    puts("�ʵe�����{��V2015.2.5 by Cliff Wu");
    puts("*********************************");
    puts("�γ~�G�۰ʱN�ʵe�B�v���̥H�U�W�h������J��Ƨ�");
    puts("�P�_�W�h�G���ƫe���r���@��Ƨ��W��");
    puts("�d�ҡG");
    puts("       \"[SumiSora][Sword_Art_Online][01][GB][720p].mp4\"");
    puts("       �۰ʲ��͸�Ƨ�\"[SumiSora][Sword_Art_Online]\"�A�ñN�W�z�ɮש�J");
    puts("�ϥΤ�k�G�N�����ɩ�J����z���ؿ��ð���A���G�|��ܦb�ù��ì����b\"log.txt\"");
    puts("�`�N�G�ӥؿ����U�Ф��n��m��L�ɮשθ�Ƨ��I");
    puts("�I���{���ȨѤ����ɮרϥΡA�Y������l�������t�d�I");
    puts("*******************************");
    puts("�P�N�Ы����N��}�l����Ϋ�X���}");
    system("pause>nul");
    puts("");

    // ��ܨì����t�ήɶ�
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

            // �ϥ��ɦW����'.'�Ӱ�²���P�_�O�ɮ��٬O��Ƨ�
            file = 0;
            for(i = 0; i < (ep->d_namlen); i++)
            {
                if(ep->d_name[i] == '.')
                    file = 1;
            }
            /* �L�X�ؿ����U�Ҧ���Ƨ����ɮ�
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
                        state = 1; //�i�J��z�{��
                        // ���̥i��O���ƪ���Ӧr��
                        if(i-1 != 0)
                        {
                            strncpy(name, (ep->d_name), i-1); // �H�o��Ӧr���e���r���@��Ƨ��W��
                            //printf("%s\n", name);
                        }
                        else
                            strcpy(name, "unclassifiable");

                        // �j�M�O�_��Ƨ��w�إ�
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
                            if(find == 1) // ��Ƨ��w�s�b
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
                            else // ��Ƨ����s�b
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
        printf("�ɮ׳��w��z����\n");
        fprintf(fPtr, "All files have been classified.\n");
    }

    printf("------------------------End------------------------\n");
    fprintf(fPtr, "------------------------End------------------------\n");
    fclose(fPtr);

    printf("�B�z�����A�Ы����N�䵲��");
    system("pause>nul");
    return 0;
}
