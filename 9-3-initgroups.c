/*Exercise 9-3
 * Implement initgroups() using setgroups() and library functions for retreiving 
 * information from the password and group files (Section 8.4). Remember that a 
 * process must be privileged in order to be able to call setgroups().
 */
 
 
#include <unistd.h>
#include <sys/fsuid.h>
#include <grp.h>
#include "tlpi_hdr.h"
#include "ugid_functions.h"   /* userNameFromId() & groupNameFromId() */


#define GROUPS_MAX  1000
//int setgroups(size_t gidsetsize, const gid_t *grouplist);
int _initgroups(const char *user, gid_t group);
/* TLPI pg 179
 initializes the calling process's supplementary group IDs by scanning /etc/group
 and building a list of all groups of which the named user is a member. In addition,
 the group ID specified in group is also added to the process's set of supplementary 
 group IDs
*/
void printgroups(void);


int _initgroups(const char *user, gid_t group){
   gid_t list[GROUPS_MAX];
   
   int ngroups = GROUPS_MAX;
   int numgroups = getgrouplist(user, group, list, &ngroups);
   
   setgroups(numgroups, list);
   return 0;
}
void printgroups(){
   gid_t list[GROUPS_MAX];
   
   int numgroups = getgroups(GROUPS_MAX, list);
   printf("printing groups: \n");
   
   for(int i = 0;i <numgroups;i++){
      printf("%d : %s\n", list[i], groupNameFromId(list[i]) );
   }
   
   
   
}
int main(int argc, char *argv[]){
   printgroups();
   gid_t newgroups[] = {1,2};
   setgroups(2, newgroups);
   printgroups();

   
   _initgroups("munki", 1004);
   printgroups();

   initgroups("root", 0);
   printgroups();
   
    
}
