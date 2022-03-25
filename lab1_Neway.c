
/* Name:     Neway Moges
   Due date: 02/13/2022
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/utsname.h>

//convert time that given with seconds into 
// year, months, days, hours, minutes and seconds
int Sec_to_year_Converter(int inputS)
{
    int sec, hh, mm, dd, mon, year;
	 year = inputS/31540000;
     inputS -= year * 31540000;
     mon = inputS/2628000;
     inputS -= mon * 2628000;
	 dd = inputS/(86400);
     inputS -= dd * (86400); 
	 hh = inputS/3600;
     inputS -= hh * 3600;
	 mm = inputS/60;
     inputS -= mm * 60;
     sec = inputS;
     //since 1970 so we need to add the year
     year +=1970;
	 printf("%d-%d-%d  %d:%d:%d", year, mon, dd, hh, mm, sec);

     return(0);
}

//convert time from seconds to days and lower
int timeConverter(double inputS)
{
    int sec, hh, mm, dd;
	 
	 dd = inputS/(86400);
     inputS -= dd * (86400); 
	 hh = inputS/3600;
     inputS -= hh * 3600;
	 mm = inputS/60;
     inputS -= mm * 60;
     sec = inputS;

	 printf("%d:%d:%d:%d", dd, hh, mm, sec);

     return(0);
}

double amntOf_time_since_last_booted()
{
    FILE* fp;
    char buffer[1024];
    size_t bRead;
    
    double Cspeed;

    //read everything from proc/uptime to the variable name buffer
    fp = fopen ("/proc/uptime","r");
    bRead = fread (buffer, 1, sizeof (buffer), fp);
    fclose (fp);

    //exit if it failed to read
    if (bRead == 0){
        printf("Sorry... fail to read!\n");
        return(0);
    }

    // terminate with null
    buffer[bRead] = '\0';

    sscanf (buffer, "%lf", &Cspeed);
    return Cspeed;
}

// A function to read the total memory and how much memory is available
int memTotl_memAvlble ()
{
    FILE* fp;
    char buffer[1500];
    size_t bRead;
    char* match1;
    char* match2;
    int totalM, AvailableM;

    // Read everything from proc/meminfo into variable called buffer
    fp = fopen ("/proc/meminfo","r");
    bRead = fread (buffer, 1, sizeof (buffer), fp);
    fclose (fp);     //close the file

    // exit if failed to read
    if (bRead == 0){
        printf("Sorry... fail to read!\n");
        return(0);
    }
    //terminate with null 
    buffer[bRead] = '\0';

    // go through buffer and locate the name "MemTotal"
    // using strstr( search string for substring) 
    match1 = strstr (buffer, "MemTotal:");

    //check if the variables has any value
    if (match1 == NULL){
        printf("Nothing is there");
        return(0);
    }

    match2 = strstr (buffer, "MemAvailable:");
    if (match1 == NULL){
        printf("Nothing is there");
        return(0);
    }
    
    // after locating the names Parse the line to extract the total memory and how much is left.
    sscanf (match1, "MemTotal: %d", &totalM);
    sscanf (match2, "MemAvailable: %d", &AvailableM);

    // Display the values on the screen
    printf("\nTotal Memory: %d KB\n", totalM);
    printf("\nAvailable Memory: %d KB\n", AvailableM);

    return(0);
}

int system_was_last_booted ()
{
    FILE* fp;
    char buffer[1500];
    size_t bRead;
    char* match1;
    char* match2;
    int Cspeed;
    long int usrMode;
    int rndmV, systMode;

    //read contents of /proc/stat into the buffer
    fp = fopen ("/proc/stat","r");
    bRead = fread (buffer, 1, sizeof (buffer), fp);
    fclose (fp);   //close the open file

    //exit if read failed 
    if (bRead == 0){
        printf("Sorry... fail to read!\n");
        return(0);
    }

    //txt null terminate it
    buffer[bRead] = '\0';
    
    /* using strstr( search string for substring) */
    match1 = strstr (buffer, "btime");
    if (match1 == NULL){
        printf("Nothing is there");
        return 0;
    }

    match2 = strstr (buffer, "cpu");
    if (match2 == NULL){
        printf("Nothing is there");
        return 0;
    }
    
    //scan the lines to extract the clock speed, user mode, and system mode
    sscanf (match1, "btime %d", &Cspeed);

    sscanf (match2, "cpu  %ld%d%d", &usrMode,&rndmV,&systMode);

    //Print out the time in user mode and system mode into the screen
    printf("\nAmount of time that the CPU has spent in user mode: %ld \n",usrMode);
    printf("\nAmount of time that the CPU has spent in system mode: %d \n",systMode);
    printf ("\nThe time when system was last booted(yyyy-mm-dd  hh:mm:ss):  ");
    Sec_to_year_Converter(Cspeed);     // a function call to format the time
    return (0);   
}

int main ()
{
    //Part A: function named uname to get system information.
    struct utsname buffer;

   errno = 0;
   if (uname(&buffer) < 0) {
      perror("uname");
      exit(EXIT_FAILURE);
   }

   printf("System name - %s\n", buffer.sysname);
   printf("Nodename   - %s\n", buffer.nodename);
   printf("Release     - %s\n", buffer.release);
   printf("Version     - %s\n", buffer.version);
   printf("Machine     - %s\n", buffer.machine);

    /*Part B: used proc file system to prove information about the system
     the time when the system was last booted
     in the format of yyy-mm-dd  hh:mm:ss */
    system_was_last_booted();
    
    //the amount of time since system was last booted 
    // in the format of  dd: hh: mm: ss
    printf("\nThe amount of time when the system was last booted(dd:mm:ss): ");
    timeConverter(amntOf_time_since_last_booted());

    //Call a function to identify how much memory is available
    //as well as to identify the total memory of the system
    printf("\n");
    memTotl_memAvlble();

    return 0;
}
