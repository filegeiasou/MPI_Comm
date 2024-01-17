//Αρχικοποιω τις βιβλιοθηκες που θα χρειαστω
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <assert.h>

int main(int argc, char** argv) {
    
    //Αρχικοποιω το MPI με ποσες διεργασιες θα τρεξει και τις παραμετρους που δινονται απο τον χρηστη
    MPI_Init(&argc, &argv);

    int n;
    int num;
    float m1;

    float *D=(float *)malloc(100*sizeof(float));

    int size;
    int rank;
    MPI_Status status;

    //Παιρνω τον αριθμο των διεργασιων και το rank της καθε μιας
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //Αρχικοποιω τα tags που θα χρησιμοποιησω
    int tag1 = 50, tag2 = 60, tag3 = 70 ,tag4 = 80, tag5 = 90 , tag6 = 100;
    int target;
    int flag;


    int *arr=(int *)malloc(100*sizeof(int));
    int *data_loc=(int *)malloc(100*sizeof(int));
    int flag1 = 1;

    //Μενου για τον υπολογισμο των τιμων
    while(flag1!=2)
    {
        if(flag1==1)
        {
            if(rank==0)
            {
                //Ελεγχος αν ειναι σωστος ο αριθμος των διεργασιων
                do
                {
                    printf("Give how many elements you want to send: \n");
                    scanf("%d", &n);
                } while (n%size!=0);
                
                printf("Give the numbers Xi: ");
                for(int i = 0; i < n; i++)
                {
                    scanf("%d", &arr[i]);
                }

                //Στελνω το n σε ολες τις διεργασιες
                for(target = 1; target < size; target++)
                {
                    MPI_Send(&n, 1, MPI_INT, target, tag1, MPI_COMM_WORLD);
                }

                num = n/size;
                int k = num;

                //Στελνω τα στοιχεια του πινακα σε ολες τις διεργασιες
                for(target = 1; target < size; target++)
                {
                    MPI_Send(&arr[k], num, MPI_INT, target, tag2, MPI_COMM_WORLD);
                    k+=num;
                }

                //Δημιουργω εναν local πινακα και αντιγραφω τα στοιχεια του πινακα σε αυτον
                for(k = 0; k < num; k++)
                {
                    data_loc[k] = arr[k];   
                }

            }
            //Οι υπολοιπες διεργασιες περιμενουν να λαβουν το n και τα στοιχεια του πινακα
            else
            {
                MPI_Recv(&n, 1, MPI_INT, 0, tag1, MPI_COMM_WORLD, &status);
                num = n/size;
                MPI_Recv(&data_loc[0], num, MPI_INT, 0, tag2, MPI_COMM_WORLD, &status);
            }


            //erwthma 1
            int m = 0;
            int finres;

            //Υπολογιζω το αθροισμα καθε process
            for(int i = 0; i < num; i++)
            {
                m += data_loc[i];
            }
            //Στελνω το αθροισμα στην process 0
            if(rank!=0)
            {
                MPI_Send(&m, 1, MPI_INT, 0, tag3, MPI_COMM_WORLD);
            }
            else
            {
                finres = m;
                printf("\n Apotelesma of process %d: %d\n", rank, m);

                //Λαμβανω το αθροισμα απο τα υπολοιπα process και τα προσθετω μεταξυ τους
                for(int source = 1; source < size; source++)
                {
                    MPI_Recv(&m, 1, MPI_INT, source, tag3, MPI_COMM_WORLD, &status);
                    finres += m;
                    printf("\n Apotelesma of process %d: %d\n", source, m);
                }
                //Υπολογιζω το μεσο ορο
                m1 = (float)finres / (float)n;
                printf("\n\n\n Teliko apotelesma %.2f\n", m1);
            }
            

            //erwthma 2
            int max = 0;
            int max1;

            //Υπολογιζω το μεγιστο καθε process
            for(int i = 0; i < num; i++)
            {
                if(data_loc[i] > max)
                {
                    max = data_loc[i];
                }
            }

            if(rank == 0)
            {
                max1 = max;
                //Για καθε process λαμβανω το μεγιστο και το συγκρινω με το max1
                for (int i = 1; i < size; i++) 
                {
                    int temp_max;
                    MPI_Recv(&temp_max, 1, MPI_INT, i, tag1, MPI_COMM_WORLD, &status);
                    if(temp_max > max1)
                    {
                        max1 = temp_max;
                    }
                }
                printf("Max: %d\n\n", max1);
                //Στελνω το max1 και το μεσο ορο σε ολα τα process
                for (int i = 1; i < size; i++) 
                {
                    MPI_Send(&max1, 1, MPI_INT, i, tag1, MPI_COMM_WORLD);
                    MPI_Send(&m1, 1, MPI_FLOAT, i, tag2, MPI_COMM_WORLD);
                }
            }
            //Στελνω το μεγιστο καθε process και λαμβανω το max1 και το μεσο ορο απο το process 0
            else 
            {
                MPI_Send(&max, 1, MPI_INT, 0, tag1, MPI_COMM_WORLD);
                MPI_Recv(&max1, 1, MPI_INT, 0, tag1, MPI_COMM_WORLD, &status);
                MPI_Recv(&m1, 1, MPI_FLOAT, 0, tag2, MPI_COMM_WORLD, &status);
            }

            //erwthma 3
            float var = 0.0;
            float finres1;
            float var1;

            //Υπολογιζω το τετραγωνο της διαφορας της τιμης με τον μεσο ορο του καθε process
            for(int i = 0; i < num; i++)
            {
                var = var + (((float)data_loc[i] - m1) * ((float)data_loc[i] - m1));
            }
            
            //Στελνω στο process 0 το τετραγωνο της διαφορας της τιμης με τον μεσο ορο του καθε process 
            if(rank!=0)
            {
                MPI_Send(&var, 1, MPI_FLOAT, 0, tag5, MPI_COMM_WORLD);
            }
            else
            {
                finres1 = var;
                printf("\n Apotelesma of process %d: %.2f\n", rank, var);

                //Λαμβανω το τετραγωνο της διαφορας της τιμης με τον μεσο ορο του καθε process και τα προσθετω μεταξυ τους
                for(int source = 1; source < size; source++)
                {
                    MPI_Recv(&var, 1, MPI_FLOAT, source, tag5, MPI_COMM_WORLD, &status);
                    finres1 += var;
                    printf("\n Apotelesma of process %d: %.2f\n", source, var);
                }
                //Υπολογιζω την διασπορα 
                var1 = finres1 / n;
                printf("\n\n\n Teliko apotelesma %.2f\n", var1);
            }
            
            //erwthma 4
            int p=0;
            for(int i = 0; i < num; i++) 
            {
                //Υπολογιζω το τετραγωνο της διαφορας της τιμης με το μεγιστο του καθε process
                D[i] = pow(abs(data_loc[i] - max1), 2);
                if(rank==0)
                {
                    printf("\n Apotelesma of D[%d] : %.2f\n", p,D[i]);
                    p++;
                }
                //Στελνω το τετραγωνο της διαφορας της τιμης με το μεγιστο του καθε process στο process 0
                if(rank!=0)
                {
                    MPI_Send(&D[i], 1, MPI_FLOAT, 0, tag4, MPI_COMM_WORLD);
                }
            }
            if(rank==0)
            {
                for(int source = 1; source < size; source++)
                {
                    //Λαμβανω το τετραγωνο της διαφορας της τιμης με το μεγιστο του καθε process
                    for(int i = 0; i < num; i++)
                    {
                        MPI_Recv(&D[i], 1, MPI_FLOAT, source, tag4, MPI_COMM_WORLD, &status);
                        printf("\n Apotelesma of D[%d] : %.2f\n", p,D[i]);
                        p++;
                    }
                }
            }

            if(rank==0)
            {
                //Ξαναρωταω τον χρηστη αν θελει να συνεχισει
                sleep(3);
                printf("Do you want to continue? (1/2)\n");
                scanf("%d",&flag1);
                //Στελνω σε ολα τα process το flag1
                for (int i = 1; i < size; i++) 
                {
                    MPI_Send(&flag1, 1, MPI_INT, i, tag6, MPI_COMM_WORLD);
                }        
            }
            //Λαμβανω το flag1 απο το process 0
            else
            {
                MPI_Recv(&flag1, 1, MPI_INT, 0, tag6, MPI_COMM_WORLD, &status);
            }
        }
    }
    //Τερματιζω το MPI
    MPI_Finalize();
    //Αποδεσμευω την μνημη
    free(D);
    free(arr);
    free(data_loc);
    return 0;
}
