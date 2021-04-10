#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "hash.h"
#include<sys/wait.h>
#include<unistd.h>
#include<pthread.h>


typedef struct _country{
    char name[20]; 
    char regionIndex; 
    int dailyTravelers; 
    int totalPopulation; 
    float highClass; 
    float middleClass; 
    float lowerClass; 
    float middleClassLimit; 
    float lowerClassLimit; 
    float buyers; 
    float markets; 
    int domesticsWorkers; 
    int closeAirports; 
    int startQuarentine; 
    int closeBusiness; 
    int closeMarkets;
    int closeTransport; 
    int * alliedCountries; 
    int numberAlliedCountries; 

} country; 

typedef struct _region {
    char name[10]; 
    int index; 
    int countries[100]; 
    int countriesCount; 
    
} region; 

typedef struct _model {
    char name[20];
    int  duration; 
    float spreadRate; 
    float mortality; 
    char firstPacientLoc[20];
    char firstPacientClass[5];
    char infectionDate[10]; 
    int numberRegions;
    struct _region regions[20]; 
    str2IntHashTable regionHT; 
    int regions2countries[20][100]; 
    int alliedRegions[20][20]; 
    int alliedCountries[100][100]; 
    struct _country countries[100]; 
    int numberCountries; 
    
} model;  


int getRegionIndex(struct _region regions[20], int regionSize, char * name){
    for(int i = 0; i < regionSize; ++i){
        if (!strcmp(regions[i].name, name)){
            printf("Index found: %d, %s, %s\n", i, regions[i].name, name);
            return i; 
        }
    }
    return 0; 
}

int * getAlliedRegions(int adjMatRegions[][20],int regionNumber,  int index){
    int * alliedRegions = malloc(sizeof(int)*regionNumber); 
    int j = 0; 
    for (int i = 0 ; i < regionNumber; ++i){
        if (adjMatRegions[index][i]){
            alliedRegions[j] = i;
            ++j; 
        }

    }

    return alliedRegions; 
}

int * addSublists(int * l1, int s1, int * l2, int s2){


    int * s3 = malloc(sizeof(int)*(s1 + s2)); 

    int i =0; 
    for(; i < s1; ++i){
        s3[i] = l1[i]; 
    }

    int j = 0; 
    for(; j < s2; ++j)
    {
        s3[i + j] = l2[j];

    }

    return s3; 
}

int * getCountriesByRegion( model sim, int regionIndex, int * size){
    int * countries = malloc(sizeof(int)*sim.numberCountries); 
    int j = 0; 
    for(int i =0 ;i < sim.numberCountries; ++i){
        if (sim.regions2countries[regionIndex][i] == 1){
            countries[j] = i; 
            ++j; 
        }
    }
    *size = j; 
    return countries; 
}

int * getCountriesByRegionExcept( model sim, int regionIndex, int * size, int countryIndex){
    int * countries = malloc(sizeof(int)*sim.numberCountries); 
    int j = 0; 
    for(int i =0 ;i < sim.numberCountries; ++i){
        if (sim.regions2countries[regionIndex][i] == 1 && i != countryIndex){
            countries[j] = i; 
            ++j; 
        }
    }
    *size = j; 
    return countries; 
}

model readInputFile(){

    FILE * fp; 
    char c[1000]; 

    if(((fp = fopen("Ebola.txt", "r")) == NULL)){
        printf("Error opening file!"); 
        exit(1); 
    }

    model * sim = malloc(sizeof(model)); 
    initializeTable(&sim->regionHT); 

    fscanf(fp, "%s", sim->name);
    fscanf(fp, "%d", &sim->duration); 
    fscanf(fp, "%f", &sim->spreadRate); 
    fscanf(fp, "%f", &sim->mortality);
    fscanf(fp, "%s %s %s", sim->firstPacientLoc, sim->firstPacientClass, sim->infectionDate); 
    fscanf(fp, "\t%d\n", &sim->numberRegions);


    
    char line[100]; 
    fgets(line, 20, fp); 
    int i = -1; 
    for( int x = 0; x < sim->numberRegions; ++x)
    {
        
        char regionName[20]; 

        sscanf(line, "%s\n", regionName);


        if (!existsKey(sim->regionHT, regionName)){
            ++i; 
            addStr2IntNode( &sim->regionHT, regionName, i); 
        }


        fgets(line, 20, fp); 
    
        int j = 0;
        while(line[0] == '\t' && line[0] != ' '){
            char alliedRegionName[20]; 
            sscanf(line, "\t%s\n", alliedRegionName);

            int index; 
            if (!existsKey(sim->regionHT, alliedRegionName)){
                i++; 
                addStr2IntNode(&sim->regionHT, alliedRegionName, i);
                index = i; 
            }else{
                index = getValue(sim->regionHT, alliedRegionName); 
            }


            sim->alliedRegions[x][index] = 1; 
            



            fgets(line, 20, fp); 
            ++j; 
        
        }

        
     }

    printTable(sim->regionHT);

    for(int i = 0 ; i < sim->numberRegions; ++i){
        for(int j = 0; j < sim->numberRegions; ++j){
            printf("%d ", sim->alliedRegions[i][j]);
        }
        printf("\n"); 
    }



     i = 0; 
     while(!feof(fp)){

        fgets(line, 100, fp); 


        while (line[0] == '\r' && !feof(fp)) {
            fgets(line, 100, fp); 
        }

        if (feof(fp)){
            break; 
        }

        sscanf(line, "%[^\t\n]", sim->countries[i].name); 
        fgets(line,100, fp); 

        char regionName[20]; 
        sscanf(line, "%s %d %d\n", regionName, &sim->countries[i].dailyTravelers, &sim->countries[i].closeAirports); 


        int regionIndex = getValue(sim->regionHT, regionName); 
        sim->countries[i].regionIndex = regionIndex; 

        sim->regions2countries[regionIndex][i] = 1; 


        fgets(line, 100, fp); 
        sscanf(line, "%d %d\n", &sim->countries[i].totalPopulation, &sim->countries[i].startQuarentine); 
        fgets(line, 100, fp); 
        sscanf(line, "%f %f %d\n", &sim->countries[i].highClass, &sim->countries[i].buyers, &sim->countries[i].closeBusiness);
        fgets(line, 100, fp); 
        sscanf(line, "%f %f %f %d\n", &sim->countries[i].middleClass, &sim->countries[i].middleClassLimit, &sim->countries[i].markets, &sim->countries[i].closeMarkets); 
        fgets(line, 100, fp); 
        sscanf(line, "%f %f %d %d\n", &sim->countries[i].lowerClass, &sim->countries[i].lowerClassLimit, &sim->countries[i].domesticsWorkers, &sim->countries[i].closeTransport);


        ++i; 
     }


    sim->numberCountries = i; 
    printf("size: %d\n", sim->numberCountries);


    for(int i = 0; i < sim->numberRegions; ++i ){
        for(int j = 0 ; j < sim->numberCountries; ++j){
            printf("%d ", sim->regions2countries[i][j]);
        }
        printf("\n"); 
    }



    for(int i = 0; i < sim->numberCountries; ++i){
        int regionIndex = sim->countries[i].regionIndex;
        int * alliedRegionsCountries; 
        int prev_size = 0; 

        // obtenemos todos los paises de la region 
        int size; 
        int * countries=  getCountriesByRegionExcept(*sim,regionIndex, &size, i); 



        for(int j = 0; j < sim->numberRegions; ++j){
            
            if (sim->alliedRegions[regionIndex][j]){
                int _size = 0; 
                int * _countries =  getCountriesByRegion(*sim, j, &_size);

                alliedRegionsCountries = addSublists(alliedRegionsCountries, prev_size, _countries, _size); 
                prev_size += _size; 

            }
        }

        countries = addSublists(alliedRegionsCountries, prev_size, countries, size); 

        size += prev_size; 

        sim->countries[i].alliedCountries = countries; 
        sim->countries[i].numberAlliedCountries = size; 
        
        printf("====================\n"); 
        for(int k = 0; k < size; ++k){
            printf("\t%s\n", sim->countries[countries[k]].name); 
        }
    }

      return *sim; 

}

void * runClassSim(void * classIndex){
    int class = *((int *) classIndex);
    switch (class)
    {
    case 0:
        printf("\t Simulando clase alta\n"); 
        break;
    case 1:
        printf("\t Simulando clase media\n"); 
        break; 
    case 2:
        printf("\t Simulando clase baja\n"); 
        break; 
    
    default:
        break;
    }
}


void runCountrieSim(model sim, int countryIndex){
    country ctry = sim.countries[countryIndex]; 

    printf("Simulando %s\n", ctry.name); 

    pthread_t th[3];

    for (int i = 0; i < 3; ++i){
        pthread_create(&th[i], NULL, runClassSim, &i); 
        pthread_join(th[i], NULL); 
    }

    // for (int i =0; i < 3; ++i){
    // } 




}





void main(){
    model sim = readInputFile();

    pid_t childId; 
    int i = 0; 
    for(int i = 0; i < sim.numberCountries; ++i) { 
        
        if (fork() == 0){
            runCountrieSim(sim, i); 
            exit(0); 
        }
    }

    for(int i = 0; i < sim.numberCountries; ++i){
        wait(NULL);
    }
}   



