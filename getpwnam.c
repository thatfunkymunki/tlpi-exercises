/*Exercise 8-2
 * Implement getpwnam() using setpwent(), getpwent(), and endpwent().
 */

#include <pwd.h>
#include "tlpi_hdr.h"

struct passwd *_getpwnam(const char *name);

struct passwd *_getpwnam(const char *name){
    struct passwd *pwd;
    
    while((pwd = getpwent()) != NULL){
        if(strcmp(pwd->pw_name, name) == 0){
            endpwent();
            return pwd;
        }
    }
    endpwent();
    return NULL;
   
}

int main(int argc, char *argv[])
{
    struct passwd *pwd;

    pwd = getpwnam("munki");
    printf("getpwnam: %d: %s\n", pwd->pw_uid, pwd->pw_name);
    
    pwd = _getpwnam("synth");
    printf("_getpwnam: %d: %s\n", pwd->pw_uid, pwd->pw_name);

}
