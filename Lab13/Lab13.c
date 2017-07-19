#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <time.h>

//thanks https://stackoverflow.com/q/26616038
void printPerms(mode_t perms)
{
    printf((perms & S_IRUSR) ? "r" : "-");
    printf((perms & S_IWUSR) ? "w" : "-");
    printf((perms & S_IXUSR) ? "x" : "-");
    printf((perms & S_IRGRP) ? "r" : "-");
    printf((perms & S_IWGRP) ? "w" : "-");
    printf((perms & S_IXGRP) ? "x" : "-");
    printf((perms & S_IROTH) ? "r" : "-");
    printf((perms & S_IWOTH) ? "w" : "-");
    printf((perms & S_IXOTH) ? "x" : "-");
}

int main(int argc, char **argv)
{
    struct stat fileInfo;
    struct passwd *pwdEntry;
    struct group *grpEntry;
    uid_t uid;
    gid_t gid;
    mode_t fileMode;

    if (argc < 2)
    {
        printf("No files given\n");
        return EXIT_SUCCESS;
    }

    for (int i = 1; i < argc; i++)
    {

        //int fpStat = stat("./f.g", &fileInfo);
        int fpStat = stat(argv[i], &fileInfo);
        if (fpStat < 0)
        {
            perror("stat");
            return EXIT_FAILURE;
        }

        uid = fileInfo.st_uid;
        gid = fileInfo.st_gid;
        fileMode = fileInfo.st_mode;

        pwdEntry = getpwuid(uid);
        if (pwdEntry == NULL)
        {
            printf("No such user\n");
            return EXIT_FAILURE;
        }

        grpEntry = getgrgid(gid);
        if (grpEntry == NULL)
        {
            printf("No such group\n");
            return EXIT_FAILURE;
        }

        printf("\nInformation For File %s\n", argv[i]);
        printf("%-20s : %s\n", "name", argv[i]);
        printf("%-20s : %ld\n", "device #", (long)fileInfo.st_rdev);
        printf("%-20s : %ld\n", "i-number", (long)fileInfo.st_ino);
        printf("%-20s : %ld\n", "# of links", (long)fileInfo.st_nlink);
        printf("%-20s : %ld (%s)\n", "user ID", (long)uid, pwdEntry->pw_name);
        printf("%-20s : %ld (%s)\n", "group ID", (long)gid, grpEntry->gr_name);
        printf("%-20s : %3o ", "mode", fileInfo.st_mode);
        printPerms(fileMode);
        printf("\n");
        printf("%-20s : %lld\n", "size", (long long)fileInfo.st_size);
        printf("%-20s : %s", "access time", ctime(&fileInfo.st_atime)); //these add newlines? really?
        printf("%-20s : %s", "modification time", ctime(&fileInfo.st_mtime));
        printf("%-20s : %s", "status change time", ctime(&fileInfo.st_ctime));
        printf("\n");
    }
    return EXIT_SUCCESS;
}
