                       #include<stdio.h>
int main()
{
 //int f;
                        char P_Duration[100],processed_string[100],status[100],Student_ID[100],Job_ID[100],Date_job[100],Task_type[100],Task_String[100];
             
                        float T_submission[100];
                        FILE *fp;//ready jobs

    fp=fopen("ready.txt","a+");

                        fprintf(fp,"%s,%s,%s,%s,%s,%f,%s,%s",P_Duration,Job_ID,Task_String,Task_type,Student_ID,T_submission,processed_string,Date_job,status);
                       			fclose(fp);
return 0;
}
