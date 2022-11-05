#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Default function prototypes for the tests */

void meanTest();
void frequencyTest();
void serialTest();
void pokerTest(int n, int caller);
void pokerTestFromFile();

/* Utility helper functions for the default functions */

int q_compar (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}


static FILE *fp;

int main(int argc, const char **argv)
{
    if(argc < 2)
    {
        fprintf(stderr, "Usage: %s [arg]", argv[0]);
        exit(EXIT_FAILURE);
    }
    srand(time(0)); // set the seed to class value

    int test_number = atoi(argv[1]);

    if(test_number == 1)
        meanTest();
    else if(test_number == 2)
        frequencyTest();
    else if(test_number == 3)
        serialTest();
    else if(test_number == 4) 
    {
        int n = 1000;
        int caller = test_number;
        pokerTest(n, caller);
    }
    else if(test_number == 5)
    {
        pokerTestFromFile();
    }
}

/* Calculate and display the mean of 1000 random numbers */ 
void meanTest()
{
    int random_number, sum = 0, n = 1000;

    for(int i = 0; i < n; i++)
    {
        random_number = (int)(10.0*rand()/ (RAND_MAX + 1.0));
        sum += random_number;
    }

    double mean = sum*1.0 / n;

    printf("Mean of %d random numbers is : %.1f\n", n, mean);
    
}


/* Tabulate the percentage of each digit in 10,000 random numbers */ 
void frequencyTest()
{
    struct num_stats // will store stats of numbers 0  to 9
    {
        int num_count[10];
        double percentages[10];  
    };

    int random_number, n = 10000, number;
    struct num_stats numbers_stats;

    for(int i = 0; i < 10; i++)
        numbers_stats.num_count[i] = 0;

    for(int i = 0; i < n; i++)
    {
        random_number = (int)(10.0*rand() / (RAND_MAX + 1.0));
        number = random_number;

        numbers_stats.num_count[number] += 1;                               
    }
    
    printf("Number:\t Frequency(%%)\n");
    for(int i = 0; i < 10; i++)
    {
        numbers_stats.percentages[i] = (numbers_stats.num_count[i]*1.0 / n ) * 100;
        printf("%d\t %.2f\n", i, numbers_stats.percentages[i]);
    }
}


/* Tabulates the percentage of each pair in 10,000 random numbers */
void serialTest()
{
    int n = 10000;
    int numbers[n][2];

    struct num_stats // will store stats of numbers 00 to 99
    {
        int num_count[100];
        double percentages[100];  
    };
    struct num_stats numbers_stats;

    for(int i = 0; i < 100; i++)
        numbers_stats.num_count[i] = 0;

    int number;

    for(int i = 0; i < n; i++)
    {
        numbers[i][0] = (int)(10.0*rand() / (RAND_MAX + 1.0));
        numbers[i][1] = (int)(10.0*rand() / (RAND_MAX + 1.0));

        number = numbers[i][0]*10 + numbers[i][1];
        
        numbers_stats.num_count[number] += 1;
    }

    printf("Number Pair:\t Frequency(%%)\n");

    for(int i = 0; i < 100; i++)
    {
        numbers_stats.percentages[i] = (numbers_stats.num_count[i]*1.0 / n ) * 100;

        if(i < 10)
            printf("0%d\t\t %.2f\n", i, numbers_stats.percentages[i]);
        else
            printf("%d\t\t %.2f\n", i, numbers_stats.percentages[i]);
    }   
}


/* Tabulate cardinalities of 4-digit numbers in 1000 such random numbers */
void pokerTest(int n, int caller)
{
    int numbers[n][4];
    int num_count[5] = {0, 0, 0, 0, 0};

    if(caller == 4)
    {
        if((fp = fopen("numbers.txt", "w")) == NULL)
        {
            perror("[Error opening numbers.txt]");
            exit(EXIT_FAILURE);
        }

        for(int i = 0; i < n; i++)
        {

            for(int j = 0; j < 4; j++)
            {
                numbers[i][j] = (int)(10.0*rand() / (RAND_MAX + 1.0));
                fprintf(fp, "%d", numbers[i][j]);
            }

            fputc('\n', fp);            
        }
        fclose(fp);
    }
    else if(caller == 5)
    {
        if((fp = fopen("numbers.txt", "r")) == NULL)
        {
            perror("[Error opening numbers.txt]");
            exit(EXIT_FAILURE);
        }

        for(int i = 0; i < n; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                numbers[i][j] = fgetc(fp) - '0';
            }
                
            fgetc(fp); // read the trailing new line
        }
        fclose(fp);
    }
    
    for(int i = 0; i < n; i++)
    {
        // create and sort the sequence for easy identification
        int numi_array[] = {numbers[i][0], numbers[i][1], numbers[i][2], numbers[i][3]}; 

        qsort(numi_array, 4, sizeof(int), q_compar);

        // define boolean variables for pattern identification
        int all_equal = (numi_array[0] == numi_array[1] == numi_array[2] == numi_array[3]);
        int three_equal = (numi_array[0] == numi_array[1] == numi_array[2]) 
                            || (numi_array[1] == numi_array[2] == numi_array[3]);
        int one_pair = (numi_array[0] == numi_array[1]) || (numi_array[1] == numi_array[2]) 
                        || (numi_array[2] == numi_array[3]);

        // case I: all the same eg 4444
        if(all_equal)
            num_count[0] += 1;

        // case II: three digits the same -- first 3 or last 3 eg 4443, 3444
        else if(!all_equal && three_equal)
            num_count[1] += 1;

        // case III: two pairs are the same -- first 2 digits or last 2 digits eg 4334, 4433
        else if(numi_array[0] == numi_array[1] && numi_array[2] == numi_array[3])
            num_count[2] += 1;

        // case IV: one pair the same -- first 2 digits, mid 2 digits, or last 2 digits
        else if(!all_equal && !three_equal && one_pair)
            num_count[3] += 1;

        // case v: none identical
        else
            num_count[4] += 1;
        
    }

    char str[5][1024];

    printf("|\tGROUP\t\t\t\t\t| FREQUENCY | PERCENT |\n");
    printf("%-48s", "|i) all the same (e.g. 4444)");
    printf("|%5s%d%5s", " ", num_count[0], " ");
    printf("|%2s%.2f%%%s |\n", " ", (num_count[0]*1.0 / 100), " ");
    
    printf("%-48s", "|ii)3 digits the same (e.g. 443, 3444, 4344)");
    printf("|%5s%d%4s", " ", num_count[1], " ");
    printf("|%2s%.2f%%%s |\n", " ", (num_count[1]*1.0 / 100), " ");

    printf("%-48s", "|iii) two pairs (e.g 4334, 4433)");
    printf("|%5s%d%4s", " ", num_count[2], " ");
    printf("|%2s%.2f%%%s |\n", " ", (num_count[2]*1.0 / 100), " ");

    printf("%-48s", "|iv) one pair (e.g 4324 or 4342)");
    printf("|%5s%d%3s", " ", num_count[3], " ");
    printf("|%2s%.2f%%%s |\n", " ", (num_count[3]*1.0 / 100), " ");

    printf("%-48s", "|v) none identical.");
    printf("|%5s%d%3s", " ", num_count[4], " ");
    printf("|%2s%.2f%%%s |\n", " ", (num_count[4]*1.0 / 100), " ");
    // for(int i = 0; i < 5; i++)
    //     printf("%d\n", num_count[i]);

    
}


/* poker test but with random numbers from file */
void pokerTestFromFile()
{
    int n = 1000;
    int caller = 5;
    pokerTest(n, caller);
}
